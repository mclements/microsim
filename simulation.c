/*
The (discrete event) simulation consists of moving a population of N individuals through a set of M states from healthy to dead, and for each state, record the number of visits and the mean age of a person entering that state.
*/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <Rmath.h>
#include "pqueue.h"
#include "simulation.h"

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

static char state_labels[STATE_COUNT][32] = {"Healthy", "Localised", "Locally advanced", "DX localised", "Dead"};

static double progression_hazard_ratios[GLEASON_LEVEL_COUNT] = {1.0, 1.3874, 1.4027};
static double dx_hazard_ratios[STATE_COUNT] = {-1.0, 1.1308, 0.5900, 1.2147};

static struct {
	int visit_count;
	double age_sum; /*for computation of mean age (with minimum rounding errors)*/
} state_statistics[STATE_COUNT];

static int population_size;


void simulation_init(int n)
{
	int state;

	population_size = n;
	for (state = 0; state < STATE_COUNT; state++) {
		if (state != HEALTHY) {
			state_statistics[state].visit_count = 0;
		} else {
			state_statistics[state].visit_count = population_size;
		}
		state_statistics[state].age_sum = 0.0;
	}
}


static void schedule_initial_events(struct pqueue *queue)
{
	double t;
	
	/*localised cancer*/
	t = rweibull(exp(2.3525), 64.0218);
	if (runif(0,1) > 0.2241) {
		/*account for cure (do nothing)*/
	} else {
		pqueue_insert(LOCALISED, t, queue);
	}

	/*death*/
	t = rexp(80);
	pqueue_insert(DEAD, t, queue);
}


static int random_gleason_level(void)
{
	int level;
	
	if (runif(0,1) < 0.6812) {
		level = GLEASON_LOW;
	} else if (runif(0,1) < 0.5016) {
		level = GLEASON_MEDIUM;
	} else {
		level = GLEASON_HIGH;
	}
	return level;
}


static void handle_event(int type, double t, struct pqueue *queue) /*handle event at time t*/
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
		dtl = rweibull(shape, scale * pow(phr, -1.0 / shape));

		/*generate random time until DX Localised*/
		dhr = dx_hazard_ratios[LOCALISED];
		phr = progression_hazard_ratios[gleason_level] * dhr;
		dtd = rweibull(shape, scale * pow(phr, -1.0 / shape));

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
	state_statistics[type].visit_count++;
	state_statistics[type].age_sum += t;
	
	assert(state_statistics[type].age_sum >= 0.0); /*overflow check*/
}


void simulation_run(void)
{
	struct pqueue *queue;
	int person, event_type;
	double time;

	queue = pqueue_new();
	for (person = 0; person < population_size; person++) {
		schedule_initial_events(queue);
		while (pqueue_count(queue) > 0) {
			pqueue_remove(queue, &event_type, &time);
			handle_event(event_type, time, queue);
		}
	}
	pqueue_delete(queue);
}


void simulation_print(void)
{
	int state;
	double frequency, mean_age;
	
	printf("%-16s  %-9s  %-8s\n", "STATE", "FREQUENCY", "MEAN AGE");
	for (state = 0; state < STATE_COUNT; state++) {
		frequency = ((double) state_statistics[state].visit_count) / ((double) population_size);
		if (state_statistics[state].visit_count > 0) {
			mean_age = state_statistics[state].age_sum / ((double) state_statistics[state].visit_count);
		} else {
			mean_age = 0.0;
		}
		printf("%-16s  %9.2f  %8.2f\n", state_labels[state], frequency, mean_age);
	}
}
