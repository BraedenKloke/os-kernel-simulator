#include <stdio.h>

struct process {
	int pid;
	int arrival_time;
	int total_cpu_time;
	int io_frequency;
	int io_duration;
};

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
struct process process1;
struct process process2;

int max_runtime;

int read_from_file(char *file_name, struct process processes[]){
	FILE *file = fopen(file_name, "r"); 		// Open File
	if(file == NULL) 				// If File is non exsistant or empty return 0 and print error
	{
		printf("Error opening file or file is empty");
		return 0;
	}

	int num_processes = 0; // Initialize number of processes to 0 and use as array index
	

	// Scan file (assuming that our data is 5 integers seperated by spaces, 
	// this will get messed up if we have text in the file, if so we need to skip a line), 
	// Note that if we use a CSV file this function works the same, 
	// arguments will just need to be seperated by commas
	// fscanf returns the number of sucsessfully matched and assigned input items, therefor it should return 5 or not proceed as something went wrong

	char c;
	do {
		c = fgetc(file);
	} while (c != '\n');

	while ( fscanf (
			file, 
			"%d, %d, %d, %d, %d",              
			&processes[num_processes].pid,
			&processes[num_processes].arrival_time,
			&processes[num_processes].total_cpu_time,
			&processes[num_processes].io_frequency,
			&processes[num_processes].io_duration) == 5 ) {      
		num_processes++;
	}


	fclose(file);
	return num_processes;
}


int test_file_read(int num_processes, struct process processes[]){

	FILE *output;
	output = fopen("out/output_4.csv", "w+");

	for (int i = 0; i < num_processes; i++) {
		fprintf(output, "Process: %d,", i);
	        fprintf(output, "PID: %d,", processes[i].pid);
	        fprintf(output, "Arrival Time: %d,", processes[i].arrival_time);
	        fprintf(output, "Total CPU Time: %d,", processes[i].total_cpu_time);
	        fprintf(output, "I/O Frequency: %d,", processes[i].io_frequency);
	        fprintf(output, "I/O Duration: %d\n", processes[i].io_duration);
	    }
	fclose(output);
	return 0;
}


int main(int argc, char **argv) {
	printf("Starting Os Kernal Simulator\n");

	// 1. transform input csv to process structs and store in array
	//	Assumption: don't have to handle inputs with more than 30 processes
	
	// for each csv item
	// 	create process struct
	//	store process in processes array

	// TODO(@braedenkloke): delete, uneccesary variables
	/*
	process1.pid = 1001;
	process1.arrival_time = 0;
	process1.total_cpu_time = 100;
	process1.io_frequency = 101;
	process1.io_duration = 100;

	process2.pid = 1002;
	process2.arrival_time = 100;
	process2.total_cpu_time = 100;
	process2.io_frequency = 101;
	process2.io_duration = 100;

	processes[0] = 	process1;
	processes[1] = 	process2;
	*/

	int num_processes = read_from_file ( argv[1], processes );	
	test_file_read ( num_processes, processes ); 


	// TODO(@braedenkloke): delete, this is temporary code
	//for (int i = 0; i < 2; i++) {
	//	printf("%d\n", processes[i].pid);
	//}

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
