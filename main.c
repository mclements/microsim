#include <stdio.h>
#include <stdlib.h>
#include "simulation.h"

int main(int argc, char **argv)
{
	int population_size, exit_status;
	char *endp;
	
	if (argc == 2) {
		population_size = strtol(argv[1], &endp, 10);
		if ((*endp == '\0') && (population_size > 0)) {
			simulation_run(population_size);
			simulation_print_summary(0);
			putchar('\n');
			simulation_print_visits(0);
			exit_status = EXIT_SUCCESS;
		} else {
			fprintf(stderr, "Population size must be a positive integer\n");
			exit_status = EXIT_FAILURE;
		}
	} else {
		fprintf(stderr, "Wrong number of arguments. Synopsis: %s population-size\n", argv[0]);
		exit_status = EXIT_FAILURE;
	}
	return exit_status;
}
