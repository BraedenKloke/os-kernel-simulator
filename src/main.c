#include <stdio.h>
#include "input.h"

struct running_state_table {
	int pid;
	int running_time;
	int remaining_cpu_time;
};

struct ready_state_node {
	int pid;
	int remaining_cpu_time;
	struct ready_state_node *next;
};

struct process processes[2];

int max_runtime;

// TODO(@braedenkloke): refactor to seperate file
// TODO(@breadenkloke): update headers / columns to reflect final deliverable
//	- fill with dummy data
int test_file_read(int num_processes, struct process processes[]){
	
	FILE *output;
	output = fopen("out/output_4.csv", "w+");
	printf("Time, PID, Old State, New State\n");

	for (int i = 0; i < num_processes; i++) {
		fprintf(output, "%d, ", i);
	        fprintf(output, "%d, ", processes[i].pid);
	        fprintf(output, "%d, ", i);
	        fprintf(output, "%d\n", i);
	    }
	fclose(output);
	return 0;
}

// TODO(@braedenkloke): refactor main block
int main(int argc, char **argv) {
	printf("Starting Os Kernal Simulator\n");

	// 1. transform input csv to process structs and store in array
	//	Assumption: don't have to handle inputs with more than 30 processes
	
	// for each csv item
	// 	create process struct
	//	store process in processes array

	int num_processes = read_from_file ( argv[1], processes );	
	test_file_read ( num_processes, processes ); 

	// TODO: max(arrival_time) + sum(total_cpu_time)
	max_runtime = 10000;
	

	for (int i = 0; i < max_runtime; i++) {
	
	// 2. start for loop ( i < max(arrival_time) + sum(cpu_time))
	//	Check states and move processes if appropriate
	//		note: IF a process is moved to a different state, record in output table
	//	a. "new state" check processes array for proccesses with arraival time = to current clock
	//		b. add processes from proccesses array to ready state queue FIFO
	//	b. "running state" terminate process if CPU time = 0
	//	c. if running state is empty, move process from ready to running
	//	//d. "waiting sate"

	// 	Act on tables
	// 		increment running time / decremetn CPU time on process in running state
	//		decrement waiting time on processes in waiting state
	

	

	
	}

	return 0;
}
