#include <stdio.h>
#include "input.h"
#include <stdbool.h>

const char NEW_STATE = 'n';
const char RUNNING_STATE = 'X';
const char READY_STATE = 'R';
const char WAITING_STATE = 'W';
const char TERMINATED_STATE = 'T';

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

struct output_struct {
    int time;
    int pid;
    char old_state;
    char new_state;
};

static struct output_struct output_spooler[100];

struct process processes[2];

int print_count = 1;

int max_runtime;

// TODO(@braedenkloke): refactor to seperate file
// TODO(@breadenkloke): update headers / columns to reflect final deliverable
//	- fill with dummy data
int print_output_spooler(){
	printf("Inside function print_output_spooler");

    FILE *output;
    output = fopen("out/output_4.csv", "w+");
    fprintf(output, "Time, PID, Old State, New State\n");

	printf("PID: %d\n", output_spooler[0].pid);


    for(int i = 0; i < 1; i++) {
        fprintf(output, "%d, ", output_spooler[i].time);
        fprintf(output, "%d, ", output_spooler[i].pid);
        fprintf(output, "%c, ", output_spooler[i].old_state);
        fprintf(output, "%c\n", output_spooler[i].new_state);
    }
    fclose(output);
    print_count += 1;
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

	// TODO: max(arrival_time) + sum(total_cpu_time)
	// TODO: fix segmentation fault error, error occurs when max_runtime = 173
	max_runtime = 100;
	

	for (int clock = 0; clock < max_runtime; clock++) {
		printf("Clock: %d\n", clock);	
		// 2. start for loop ( i < max(arrival_time) + sum(cpu_time))
		//	Check states and move processes if appropriate
		//		note: IF a process is moved to a different state, record in output table
		//	a. "new state" check processes array for proccesses with arraival time = to current clock
		//		b. add processes from proccesses array to ready state queue FIFO
		//	b. "running state" terminate process if CPU time = 0
		//	c. if running state is empty, move process from ready to running
		//	//d. "waiting state"


		// Ready Queue
		for (int i = 0; i < num_processes; i++) {
			if (processes[i].arrival_time == clock) {
				ready_state.queue[ready_state.last] = processes[i];
				ready_state.last++;
                		output_spooler[0].time = clock;
				output_spooler[0].pid = processes[i].pid;
				output_spooler[0].old_state = NEW_STATE;
				output_spooler[0].new_state = READY_STATE;
				printf("PID: %d\n", output_spooler[0].pid);
			}
		}


		// Terminate Processes
		if(running.process_is_running == true && running.remaining_cpu_time == 0){
		    running.process_is_running = false;
		}

		// Fill Running State
		if(running.process_is_running == false){
		    running.running_process = ready_state.queue[ready_state.first];
		    ready_state.first++;
		    running.running_time = 0;
		    running.remaining_cpu_time = running.running_process.total_cpu_time;
		}

		// Tick Clock for running processes and soon to be waiting processes
		if(running.process_is_running == true && running.remaining_cpu_time > 0){
		    running.running_time += 1;
		    running.remaining_cpu_time -= 1;
		}

		// 	Act on tables
		// 		increment running time / decremetn CPU time on process in running state
		//		decrement waiting time on processes in waiting state
	}
	printf("End of for loop.");
	printf("About to print output spooler ...");
	print_output_spooler();

	return 0;
}
