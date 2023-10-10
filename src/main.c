#include <stdio.h>
#include "input.h"
#include <stdbool.h>

const char NEW_STATE = 'N';
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
int output_spooler_count = 0;

struct process processes[2];

int max_runtime;

// TODO(@braedenkloke): refactor to seperate file
// TODO(@breadenkloke): update headers / columns to reflect final deliverable
//	- fill with dummy data
int print_output_spooler(){
	printf("Inside function print_output_spooler\n");

    FILE *output;
    output = fopen("out/output_4.csv", "w+");
    fprintf(output, "Time, PID, Old State, New State\n");

//	printf("PID: %d\n", output_spooler[0].pid);


    for(int i = 0; i < output_spooler_count; i++) {
        fprintf(output, "%d, ", output_spooler[i].time);
        fprintf(output, "%d, ", output_spooler[i].pid);
        fprintf(output, "%c, ", output_spooler[i].old_state);
        fprintf(output, "%c\n", output_spooler[i].new_state);
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

	// TODO: max(arrival_time) + sum(total_cpu_time)
	// TODO: fix segmentation fault error, error occurs when max_runtime = 173
	max_runtime = 150;
	

	for (int clock = 0; clock < max_runtime; clock++) {
		//printf("Clock: %d\n", clock);	
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
                output_spooler[output_spooler_count].time = clock;
				output_spooler[output_spooler_count].pid = processes[i].pid;
				output_spooler[output_spooler_count].old_state = NEW_STATE;
				output_spooler[output_spooler_count].new_state = READY_STATE;
				printf("PID: %d\n", output_spooler[output_spooler_count].pid);
				output_spooler_count++;
			}
		}


		// Terminate Processes
		if(running.process_is_running == true && running.remaining_cpu_time == 0){
		    running.process_is_running = false;
		    output_spooler[output_spooler_count].time = clock;
		    output_spooler[output_spooler_count].pid = running.running_process.pid;
		    output_spooler[output_spooler_count].old_state = RUNNING_STATE;
		    output_spooler[output_spooler_count].new_state = TERMINATED_STATE;
		    printf("PID: %d\n", output_spooler[output_spooler_count].pid);
		    output_spooler_count++;
		}

		// Fill Running State
		if(running.process_is_running == false){
		    running.running_process.pid = ready_state.queue[ready_state.first].pid;
            running.running_process.total_cpu_time = ready_state.queue[ready_state.first].total_cpu_time;
            running.running_process.arrival_time = ready_state.queue[ready_state.first].arrival_time;
            running.running_process.io_duration = ready_state.queue[ready_state.first].io_duration;
            running.running_process.io_frequency = ready_state.queue[ready_state.first].io_frequency;
		    ready_state.first++;
		    running.running_time = 0;
		    running.remaining_cpu_time = running.running_process.total_cpu_time;
			running.process_is_running = true;

		    output_spooler[output_spooler_count].time = clock;
		    output_spooler[output_spooler_count].pid = running.running_process.pid;
		    output_spooler[output_spooler_count].old_state = READY_STATE;
		    output_spooler[output_spooler_count].new_state = RUNNING_STATE;
		    output_spooler_count++;
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
	printf("About to print output spooler ...\n");
	print_output_spooler();

	return 0;
}
