#include <stdio.h>
#include "input.h"
#include <stdbool.h>

struct running_state_table {
    struct process running_process;
	int running_time;
	int remaining_cpu_time;
    bool process_is_running;
};

struct ready_state_struct {
	struct process queue[20];
	int first;
	int last;
};

struct process processes[2];

int max_runtime;

// TODO(@braedenkloke): refactor to seperate file
// TODO(@breadenkloke): update headers / columns to reflect final deliverable
//	- fill with dummy data
int test_file_read(int num_processes, struct process processes[]){
	
	FILE *output;
	output = fopen("out/output_4.csv", "w+");
	fprintf(output, "Time, PID, Old State, New State\n");

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

	int num_processes = read_from_file ( argv[1], processes );	
	
	struct ready_state_struct ready_state;

	ready_state.first = 0;
	ready_state.last = 0;
    struct running_state_table running;
    running.process_is_running = false;

	test_file_read ( num_processes, processes ); 

	// TODO: max(arrival_time) + sum(total_cpu_time)
	max_runtime = 10000;
	

	for (int clock = 0; clock < max_runtime; clock++) {
	
	// 2. start for loop ( i < max(arrival_time) + sum(cpu_time))
	//	Check states and move processes if appropriate
	//		note: IF a process is moved to a different state, record in output table
	//	a. "new state" check processes array for proccesses with arraival time = to current clock
	//		b. add processes from proccesses array to ready state queue FIFO
	//	b. "running state" terminate process if CPU time = 0
	//	c. if running state is empty, move process from ready to running
	//	//d. "waiting sate"



		for (int i = 0; i < num_processes; i++) {
			if (processes[i].arrival_time == clock) {
				ready_state.queue[ready_state.last] = processes[i];
				ready_state.last++;
			}
		}
        // TODO: terminate process in running state if cpu time = 0
        if(running.process_is_running == false){
            running.running_process = ready_state.queue[ready_state.first];
            ready_state.first++;
            running.running_time = 0;
            running.remaining_cpu_time = running.running_process.total_cpu_time;
        }

        /*
        if(running.pid != 0 && running.running_process != 0){
            running.running_time += 1;
            running.remaining_cpu_time -= 1;
        }
        */
	// 	Act on tables
	// 		increment running time / decremetn CPU time on process in running state
	//		decrement waiting time on processes in waiting state
	

	

	
	}

	return 0;
}
