/*
The (discrete event) simulation consists of moving a population of N individuals through a set of M states from healthy to dead, and for each state, record the number of visits and the mean age of a person entering that state.
*/

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dist.h"
#include "pqueue.h"
#include "simulation.h"

#define AGE_MAX 200

/*states/event types*/
#define HEALTHY 0
#define LOCALISED 1
#define LOCALLY_ADVANCED 2
#define DX_LOCALISED 3
#define DEAD 4
#define STATE_COUNT 5

/*gleason levels*/
#define GLEASON_LOW 0 /*level < 7*/
#define GLEASON_MEDIUM 1 /*level = 7*/
#define GLEASON_HIGH 2 /*level > 7*/
#define GLEASON_LEVEL_COUNT 3

const static char state_labels[STATE_COUNT][32] = {"Healthy", "Localised", "Locally advanced", "DX localised", "Dead"};

const static double progression_hazard_ratios[GLEASON_LEVEL_COUNT] = {1.0, 1.3874, 1.4027};
const static double dx_hazard_ratios[STATE_COUNT] = {-1.0, 1.1308, 0.5900, 1.2147};

typedef struct { 
	double age_sum;
	int visits[AGE_MAX + 1];
} state_statistics[STATE_COUNT];

static int population_size;
static state_statistics total_statistics;

static void schedule_initial_events(struct pqueue *queue)
{
	double t;
	
	/*localised cancer*/
	t = dist_weibull(exp(2.3525), 64.0218);
	if (dist_unif() > 0.2241) {
		/*account for cure (do nothing)*/
	} else {
		pqueue_insert(LOCALISED, t, queue);
	}

	/*death*/
	t = dist_exp(80);
	pqueue_insert(DEAD, t, queue);
}


static int random_gleason_level(void)
{
	int level;
	
	if (dist_unif() < 0.6812) {
		level = GLEASON_LOW;
	} else if (dist_unif() < 0.5016) {
		level = GLEASON_MEDIUM;
	} else {
		level = GLEASON_HIGH;
	}
	return level;
}


static void handle_event(int type, double t, struct pqueue *queue, state_statistics statistics) /*handle event at time t*/
{
	double phr, dhr; /*hazard ratios for locally advanced and dx localised*/
	double dtl, dtd; /*time increments for locally advanced and dx localised*/
	double shape, scale; /*weibull parameters*/
	int gleason_level;
	
	switch (type) {
	case LOCALISED:
		shape = exp(1.0353);
		scale = 19.8617;
		gleason_level = random_gleason_level();
		
		/*generate random time until Locally Advanced*/
		phr = progression_hazard_ratios[gleason_level];
		dtl = dist_weibull(shape, scale * pow(phr, -1.0 / shape));

		/*generate random time until DX Localised*/
		dhr = dx_hazard_ratios[LOCALISED];
		phr = progression_hazard_ratios[gleason_level] * dhr;
		dtd = dist_weibull(shape, scale * pow(phr, -1.0 / shape));

		/*schedule the event that will happen first*/		
		if (dtl > dtd) {
			pqueue_insert(LOCALLY_ADVANCED, t + dtl, queue);
		} else {
			pqueue_insert(DX_LOCALISED, t + dtd, queue);
		}

		break;
	case LOCALLY_ADVANCED:
		/*do nothing*/
		break;
	case DX_LOCALISED:
		/*do nothing*/
		break;
	case DEAD:
		pqueue_clear(queue);
		break;
	}
	statistics[type].age_sum += t;
	if ((int) t <= AGE_MAX) {
		statistics[type].visits[(int) t]++;
	} else {
		statistics[type].visits[AGE_MAX]++;
	}
	
	assert(statistics[type].age_sum >= 0.0); /*overflow check*/
}


static void init_statistics(state_statistics statistics)
{
	int state, age;

	for (state = 0; state < STATE_COUNT; state++) {
		statistics[state].age_sum = 0.0;
		for (age = 0; age <= AGE_MAX; age++) {
			statistics[state].visits[age] = 0;
		}
	}
}


static int sum(const int a[], int n)
{
	int s, i;
	
	s = 0;
	for (i = 0; i < n; i++) {
		s += a[i];
	}
	return s;
}


void simulation_run(int popsize)
{
	struct pqueue *queue;
	int person, event_type, state, age;
	double time;
	state_statistics partial_statistics;

	population_size = popsize;
	init_statistics(total_statistics);
#pragma omp parallel private (partial_statistics, queue, event_type, time, state, age)
	{
		init_statistics(partial_statistics);
		queue = pqueue_new();
#pragma omp for
		for (person = 0; person < population_size; person++) {
			schedule_initial_events(queue);
			while (pqueue_count(queue) > 0) {
				pqueue_remove(queue, &event_type, &time);
				handle_event(event_type, time, queue, partial_statistics);
			}
		}
		pqueue_delete(queue);
#pragma omp critical
		for (state = 0; state < STATE_COUNT; state++) {
			total_statistics[state].age_sum += partial_statistics[state].age_sum;
			for (age = 0; age <= AGE_MAX; age++) {
				total_statistics[state].visits[age] = partial_statistics[state].visits[age];
			}
		}
	}

	assert(sum(total_statistics[DEAD].visits, AGE_MAX + 1) == popsize);
}


void simulation_print_summary(int as_json)
{
	int state, visits;
	double frequency, mean_age;
	
	if (as_json) {
		putchar('[');
	} else {
		printf("%-16s  %-9s  %-8s\n", "STATE", "FREQUENCY", "MEAN AGE");
	}
	for (state = 1; state < STATE_COUNT; state++) {
		visits = sum(total_statistics[state].visits, AGE_MAX + 1);
		frequency = ((double) visits) / ((double) population_size);
		if (visits > 0) {
			mean_age = total_statistics[state].age_sum / ((double) visits);
		} else {
			mean_age = 0.0;
		}
		if (as_json) {
			if (state > 1) {
				putchar(',');
			}
			printf("{\"state\":\"%s\",\"freq\":%.2f,\"meanAge\":%.2f}", state_labels[state], frequency, mean_age);
		} else {
			printf("%-16s  %9.2f  %8.2f\n", state_labels[state], frequency, mean_age);
		}
	}
	if (as_json) {
		puts("]");
	}
}


void simulation_print_visits(int as_json)
{
	int label_widths[STATE_COUNT];
	int age, state;
	
	if (as_json) {
		putchar('[');
		for (age = 0; age <= AGE_MAX; age++) {
			if (age > 0) {
				putchar(',');
			}
			putchar('{');
			for (state = 1; state < STATE_COUNT; state++) {
				if (state > 1) {
					putchar(',');
				}
				printf("\"%s\":%d", state_labels[state], total_statistics[state].visits[age]);
			}
			putchar('}');
		}
		putchar(']');
	} else {
		printf("Age");
		for (state = 0; state < STATE_COUNT; state++) {
			label_widths[state] = strlen(state_labels[state]);
			if (state > 0) {
				printf("  %s", state_labels[state]);
			}
		}
		putchar('\n');
		for (age = 0; age <= AGE_MAX; age++) {
			printf("%3d", age);
			for (state = 1; state < STATE_COUNT; state++) {
				printf("  %*d", label_widths[state], total_statistics[state].visits[age]);
			}
			putchar('\n');
		}
	}
}
