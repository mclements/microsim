#ifndef SIMULATION_H
#define SIMULATION_H

void simulation_run(int population_size); /*runs the simulation*/

void simulation_print_summary(int as_json); /*prints frequency and mean age per state*/

void simulation_print_visits(void); /*print number of visits per age and state*/

#endif
