#include <stdio.h>
#include <stdlib.h>
#include "simulation.h"

int main(int argc, char **argv)
{
	int n; /*population size*/
	
	if (argc == 2) {
		n = atoi(argv[1]); /*0 on failure*/
		if (n > 0) {
			simulation_init(n);
			simulation_run();
			simulation_print();
		} else {
			fprintf(stderr, "Population size must be a positive integer\n");
			exit(EXIT_FAILURE);
		}
	} else {
		fprintf(stderr, "Wrong number of arguments. Synopsis: gf population-size\n");
		exit(EXIT_FAILURE);
	}
	return 0;
}
