#include <stdio.h>
#include <stdbool.h>
#include "constants.h"
#include "input.h"
#include "models/states/waiting.h"
#include "models/states/ready.h"
#include "models/states/running.h"
#include "models/process_control_block.h"
#include "models/output_spool_entry.h"
#include "output_spooler.h"

struct process_control_block processes[2];

int max_runtime;

int main(int argc, char **argv) {
	printf("INFO: Starting Os Kernal Simulator\n");

	int num_processes = read_from_file(argv[1], processes);	
	
	struct ready_state_struct ready_state;

	ready_state.first = 0;
	ready_state.last = 0;
	ready_state.queue_size = 0;
	struct running_state_table running;
	running.process_is_running = false;
	running.running_process.pid = -1;


	// TODO: max(arrival_time) + sum(total_cpu_time)
	max_runtime = 1000;
	

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
				printf("DEBUG: adding to ready queue PID: %d\n", processes[i].pid);
				ready_state.queue[ready_state.last] = processes[i];
				ready_state.queue_size++;
				ready_state.last++;
				
				struct output_spool_entry data;
				data = create_output_spool_entry(clock, processes[i].pid, NEW_STATE, READY_STATE);
				write_to_output_spool(data, output_spool, output_spool_size);
				output_spool_size++;
			}
		}


		// Terminate Processes
		if(running.process_is_running == true && running.remaining_cpu_time == 0){
			running.process_is_running = false;

			struct output_spool_entry data;
			data = create_output_spool_entry(clock, running.running_process.pid, RUNNING_STATE, TERMINATED_STATE);
			write_to_output_spool(data, output_spool, output_spool_size);
			output_spool_size++;
		}

		// Fill Running State
		if(running.process_is_running == false && ready_state.queue_size > 0){
			printf("DEBUG: Dispatching process from ready state to running state ...\n");

		    running.running_process.pid = ready_state.queue[ready_state.first].pid;

			printf("DEBUG: running.running_process.pid: %d\n", running.running_process.pid);
		    running.running_process.total_cpu_time = ready_state.queue[ready_state.first].total_cpu_time;
		    running.running_process.arrival_time = ready_state.queue[ready_state.first].arrival_time;
		    running.running_process.io_duration = ready_state.queue[ready_state.first].io_duration;
		    running.running_process.io_frequency = ready_state.queue[ready_state.first].io_frequency;
			ready_state.queue_size--;
		    ready_state.first++;
		    running.running_time = 0;
		    running.remaining_cpu_time = running.running_process.total_cpu_time;
			running.process_is_running = true;

			struct output_spool_entry data;
			data = create_output_spool_entry(clock, running.running_process.pid, READY_STATE, RUNNING_STATE);
			write_to_output_spool(data, output_spool, output_spool_size);
			output_spool_size++;
		}

		// Tick Clock for running processes and soon to be waiting processes
		if(running.process_is_running == true && running.remaining_cpu_time > 0){
		    running.running_time += 1;
		    running.remaining_cpu_time -= 1;
            running.running_process.total_cpu_time -= 1;
            if(running.running_time == running.running_process.io_duration){
                //go to waiting state
            }

		}




		// 	Act on tables
		// 		increment running time / decremetn CPU time on process in running state
		//		decrement waiting time on processes in waiting state




	}
	write_output_spool_to_file(output_spool, "out/output_4.csv");

	return 0;
}
