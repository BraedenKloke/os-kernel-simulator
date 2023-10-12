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
	struct waiting_state waiting;
	struct running_state_table running;

	waiting.wait_position = 0;

	ready_state.first = 0;
	ready_state.last = 0;
	ready_state.queue_size = 0;

	running.process_is_running = false;

	// TODO: max(arrival_time) + sum(total_cpu_time)
	max_runtime = 10000;
	

	for (int clock = 0; clock < max_runtime; clock++) {

		/*
			Check States:
				1. Running -> Terminated
				2. Running -> Waiting
				3. Waiting -> Ready
				4. New -> Ready
				5. Ready -> Running
			Tick Clock:
				- increment clock
				Running State
				- running time ++
				- remaining cpu time --
				Waiting state (each process waiting)
				- waiting time ++
		*/
		// Running -> Terminated
		if(running.process_is_running == true && running.process.remaining_cpu_time == 0){
			running.process_is_running = false;

			struct output_spool_entry data;
			data = create_output_spool_entry(clock, running.process.pid, RUNNING_STATE, TERMINATED_STATE);
			write_to_output_spool(data, output_spool, output_spool_size);
			output_spool_size++;
		}
		
		// Running -> Waiting
		if(running.process_is_running == true && running.process.running_time == running.process.io_frequency){
			printf("DEBUG: Moving process from running to waiting with PID: %d\n", running.process.pid);
			
			running.process_is_running = false;
			running.process.is_waiting = true;
			waiting.process[waiting.wait_position] = running.process;
			waiting.wait_position++;

			struct output_spool_entry data;
			data = create_output_spool_entry(clock, running.process.pid, RUNNING_STATE, WAITING_STATE);
			write_to_output_spool(data, output_spool, output_spool_size);			
			output_spool_size++;
		}

		// New -> Ready 
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

		// Waiting -> Ready
		for(int i = 0; i < waiting.wait_position; i++) {
			if(waiting.process[i].is_waiting == true) {
				if(waiting.process[i].waiting_time == waiting.process[i].io_duration){
					printf("DEBUG: Moving process from waiting to ready with PID: %d\n", waiting.process[i].pid);
					waiting.process[i].waiting_time = 0;	
					waiting.process[i].is_waiting = false;
					ready_state.queue[ready_state.last] = waiting.process[i];
					ready_state.queue_size++;
					ready_state.last++;
					
					struct output_spool_entry data;
					data = create_output_spool_entry(clock, ready_state.queue[ready_state.last - 1].pid, WAITING_STATE, READY_STATE);
					write_to_output_spool(data, output_spool, output_spool_size);
					output_spool_size++;
				}
			}
		}

		// Ready -> Running
		if(running.process_is_running == false && ready_state.queue_size > 0){
			printf("DEBUG: Dispatching process from ready to running with PID: %d\n", ready_state.queue[ready_state.first].pid);

			running.process = ready_state.queue[ready_state.first];


			ready_state.queue_size--;
		    	ready_state.first++;

		    	running.process.running_time = 0;
			running.process_is_running = true;

			struct output_spool_entry data;
			data = create_output_spool_entry(clock, running.process.pid, READY_STATE, RUNNING_STATE);
			write_to_output_spool(data, output_spool, output_spool_size);
			output_spool_size++;
		}

		// Tick Clock for running processes and soon to be waiting processes
		if(running.process_is_running == true && running.process.remaining_cpu_time > 0){
		    running.process.running_time += 1;
		    running.process.remaining_cpu_time -= 1;
		}

		for(int i = 0; i < waiting.wait_position; i++){
			waiting.process[i].waiting_time++;
		}


		// 	Act on tables
		// 		increment running time / decremetn CPU time on process in running state
		//		decrement waiting time on processes in waiting state




	}
	write_output_spool_to_file(output_spool, "out/output.csv", output_spool_size);

	return 0;
}
