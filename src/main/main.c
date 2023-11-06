// TODO(@braeden): update header docs to reflect new assignment
/*****************************************************
*      SYSC4001 - F2022 - Assignment 1 Solution      *
******************************************************
* By: Ben Earle (BenEarle@cmail.carleton.ca)         *
******************************************************
* This sample solution was prepared for Dr. Wainer   *
* in fall of 2022. This solution uses a linked list  *
* to store the each states processes. They are       *
* scheduled in a FIFO manner. If you have any        *
* questions please do not hesitate to reach out via  *
* email or during the scheduled lab times.           *
******************************************************/

// Header file for input output functions
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include <assert.h>

// Macro to return the min of a and b
#define min(a, b) (((a) < (b)) ? (a) : (b))
#define NUM_MEMORY_PARTITIONS 4
#define NOT_USING_MEMORY_SCHEMA true

// An enumerator (enum for short) to represent the state
enum STATE {
    STATE_NEW,
    STATE_READY,
    STATE_RUNNING,
    STATE_WAITING,
    STATE_TERMINATED
};
static const char *STATES[] = { "NEW", "READY", "RUNNING", "WAITING", "TERMINATED"};

// A structure containing all the relevant metadata for a process, this is the PCB like struct
// The io_time_remaining is used in two ways:
// it counts how long until the next io call and how long until a current io call is complete
struct process {
    int pid;
    int arrival_time;
    int total_cpu_time;
    int cpu_time_remaining;
    int io_frequency;
    int io_duration;
    int priority;
    int process_size;
    int index_of_memory_partition;
    int io_time_remaining;
    enum STATE s;
};

// This structure represents a partition of main memory.
struct memory_partition {
    int size;
    bool is_available;
};

// This structure is a linked list of processes
// This linked list was adapted from the code presented in the following tutorial:
// https://www.hackerearth.com/practice/data-structures/linked-list/singly-linked-list/tutorial/#:~:text=In%20C%20language%2C%20a%20linked,address%20of%20the%20next%20node.
struct node {
    struct process *p;
    struct node *next;
};

// typedefs are a short hand to make the code more legible
// Here we use type def to create types for pointers to the preciously defined structures
typedef struct process *proc_t;
typedef struct node *node_t;

/* FUNCTION DESCRIPTION: create_proc
* This function creates a new process structure.
* The parameters are self descriptive:
*    -pid
*    -arrival_time
*    -total_cpu_time
*    -io_frequency
*    -io_duration
* The return value is a pointer to new process structure
*/
proc_t create_proc(int pid, int arrival_time, int total_cpu_time, int io_frequency, int io_duration, int priority, int process_size){
    // Initialize memory
    proc_t temp;
    temp = (proc_t) malloc(sizeof(struct process));

    // Initialize contents
    // The cpu time remaining starts at total CPU time
    // the state starts as new
    temp->pid=pid;
    temp->arrival_time = arrival_time;
    temp->total_cpu_time = total_cpu_time;
    temp->cpu_time_remaining = total_cpu_time;
    temp->io_frequency = io_frequency;
    temp->io_duration = io_duration;
    temp->priority = priority;
    temp->process_size = process_size;
    temp->io_time_remaining = io_frequency;
    temp->index_of_memory_partition = 0;
    temp->s = STATE_NEW;
    return temp;
}

/* FUNCTION DESCRIPTION: create_node
* This function creates a new  list node.
* The parameters are:
*    -p, a pointer to the process structure to be stored in this node
* The return value is a pointer to the new node
*/
node_t create_node(proc_t p){
    // Initialize memory
    node_t temp;
    temp = (node_t) malloc(sizeof(struct node));

    // Initialize contents
    temp->next = NULL;
    temp->p = p;

    return temp;
}

/* FUNCTION DESCRIPTION: print_nodes
* Prints all the nodes in head, along with their time remianing and current states
*/
void print_nodes(node_t head) {
    node_t current = head;
    proc_t p;

    if(head == NULL){
        printf("EMPTY\n");
        return;
    }

    while (current != NULL) {
        p = current->p;
        printf("Process ID: %d\n", p->pid);
        printf("CPU Arrival Time: %dms\n", p->arrival_time);
        printf("Time Remaining: %dms of %dms\n", p->cpu_time_remaining, p->total_cpu_time);
        printf("IO Duration: %dms\n", p->io_duration);
        printf("IO Frequency: %dms\n", p->io_frequency);
        printf("Process Priority: %dms\n", p->priority);
        printf("Current state: %s\n", STATES[p->s]);
        printf("Time until next IO event: %dms\n", p->io_time_remaining);
        printf("\n");
        current = current->next;
    }
}

/* FUNCTION DESCRIPTION: push_node
* This function adds a node to the back of the list (as though its a queue).
* The parameters are:
*    -head points to the head in the list
*    -temp is the node to be added
* The return value is a pointer to the list
*/
node_t push_node(node_t head, node_t temp){
    // prev will be used to itterate through the list
    node_t prev;

    // If the list is empty then we return a list with only the new node
    if(head == NULL){
        head = temp;
    } else {
        // Itterate through the list to add the new node at the end
        // The last node always points to NULL, so we get the next nodes until this happens
        prev = head;

        while(prev->next != NULL){
            prev = prev->next;
        }

        // Update the old final node to point to the new node
        prev->next = temp;
    }
    temp->next = NULL;
    return head;
}

/* FUNCTION DESCRIPTION: remove_node
* This function removes a node from within the linked list.
* IT DOES NOT FREE THE MEMORY ALLOCATED FOR THE NODE.
* The parameters are:
*    -head points to the pointer that is the front of the list
*    -to_be_removed points to the node that is to be removed
* The return value is an int indicating success or failure
*/
int remove_node(node_t *head, node_t to_be_removed){
    node_t temp, prev;
    if(to_be_removed == *head){
        *head = (*head)->next;
        to_be_removed->next = NULL;
        return 1;
    } else {
        temp = *head;
        // Iterate through the list until we've checked every node
        while(temp->next != NULL){
            prev = temp;
            temp = temp->next;
            if(temp == to_be_removed){
                prev->next = temp->next;
                // NOTE:Calling function must free to_be_removed when finished with it.
                // Since the addresss of the node to be removed was passed to this function
                // the calling function must already have a reference to it
                to_be_removed->next = NULL;
                return 1;
            }
        }
    }
    return -1;
}

/* FUNCTION DESCRIPTION: read_proc_from_file
* Parse the CSV input file and load its contents into a list
* The parameters are:
* The return value is a list of these new processes
*/
node_t read_proc_from_file(char *input_file){
    int MAXCHAR = 128;
    char row[MAXCHAR];
    node_t new_list=NULL, node;
    proc_t proc;
    int pid, arrival_time, total_cpu_time, io_frequency, io_duration, priority, process_size;

    FILE* f = fopen(input_file, "r");
    if(f == NULL){
        // file not opened, fail gracefully
        printf("NULL FILE\n\n\n\n");
        //  assert(false);
    }
    // Get the first row, which has the header values
    //Pid;Arrival Time;Total CPU Time;I/O Frequency;I/O Duration
    fgets(row, MAXCHAR, f);
    // Read the remainder of the rows until you get to the end of the file
    while(fgets(row, MAXCHAR, f)){
        // get the next data row
        //fgets(row, MAXCHAR, f);
        // make sure it has at least enough char to be valid
        if(strlen(row)<10) continue;
        // atoi turns a string into an integer
        // strtok(row,";") tokenizes the row around the ';' charaters
        // strtok(NULL, ";") gets the next token in the row
        // We are assuming that the file is setup as a CSV in the correct format
        pid = atoi(strtok(row, ","));
        arrival_time = atoi(strtok(NULL, ","));
        total_cpu_time = atoi(strtok(NULL, ","));
        io_frequency = atoi(strtok(NULL, ","));
        io_duration = atoi(strtok(NULL, ","));
        priority = atoi(strtok(NULL, ","));
        process_size = atoi(strtok(NULL,","));

        // We create a process struct and pass it too create node, then add this node to the new_list
        proc = create_proc(pid, arrival_time, total_cpu_time, io_frequency, io_duration, priority, process_size);
        node = create_node(proc);
        new_list = push_node(new_list, node);

    }

    return new_list;
}

/* FUNCTION DESCRIPTION: read_memory_schema_from_file
* This function reads an input memory schema file and adds the partition sizes 
* of the schema to an array representing main memory
* The parameters are:
*   - input_file: Memory schema file
*   - mm: Array representing main memory
* There is no return value.
*/
void read_memory_schema_from_file(char *input_file, struct memory_partition *mm){
    int MAXCHAR = 128;
    int count = 0;

    char row[MAXCHAR];

    FILE* f = fopen(input_file, "r");
    if(f == NULL){
        // file not opened, fail gracefully
        printf("NULL FILE\n\n\n\n");
        //  assert(false);
    }
    // Get the first row, which has the header values
    fgets(row, MAXCHAR, f);

    // Read the remainder of the rows until you get to the end of the file
    while(fgets(row, MAXCHAR, f)){
        // atoi turns a string into an integer
        // strtok(row,";") tokenizes the row around the ';' charaters
        // strtok(NULL, ";") gets the next token in the row
        // We are assuming that the file is setup as a CSV in the correct format
        mm[count].size = atoi(strtok(row, ","));
		mm[count].is_available = true;
        count++;
    }
}

/* FUNCTION DESCRIPTION: allocate_memory_partition
* This function allocates a memory partition to a given process.
* The parameters are:
*   - mm: An array representing main memory
*   - head: A pointer to the head of the new_state linked list
* There is no return value.
*/
bool allocate_memory_partition(node_t *head,struct memory_partition *mm, int verbose){
    node_t temp = *head;
    node_t node_to_be_allocated = NULL;
    int i =0;

    if(temp != NULL) {
		if(verbose) {printf("--------------------------------------------\n");}
		if(verbose) {printf("Attempting to allocate memory to process: %d\n", temp->p->pid);}
		if(verbose) {printf("Process to allocate memory to has a size of: %d Mb\n", temp->p->process_size);}
        for (i = 0; i < NUM_MEMORY_PARTITIONS; i++) {
            if(mm[i].is_available){
			if(verbose) {printf("Main memory partition %d is available with a size of: %d Mb\n", i, mm[i].size);}
                if(mm[i].size >= temp->p->process_size){
					if(verbose) {printf("Allocating memory partition %d to process: %d\n", i, temp->p->pid);}
					if(verbose) {printf("-----------------------------------------\n");}
                    mm[i].is_available = false;
                    temp->p->index_of_memory_partition = i;
                    return true;
                }
            }
        }

    }

    return false;
}

/* FUNCTION DESCRIPTION: clean_up
* This function frees all the dynamically allocated heap memory
* The parameters are:
*    - list: the list of nodes to free
*/
void clean_up(node_t list){
    node_t temp;
    while(list != NULL){
        temp = list;
        list = list->next;
        free(temp->p);
        free(temp);
    }
}

// Receives the head node of the ready list and returns the node with the highest priority (nodes with same priority are treated as equals and do not get swapped for each other)
node_t pick_highest_priority_process(node_t *head) {
    node_t temp = *head;
    node_t highest_priority_node = NULL;
    int highest_priority = -1; // Assuming priority values are positive. Adjust if they can be negative.

    while(temp != NULL) {
        if(temp->p->priority > highest_priority) {
            highest_priority = temp->p->priority;
            highest_priority_node = temp;
        }
        temp = temp->next;
    }

    return highest_priority_node;
}




int main( int argc, char *argv[]) {
    int cpu_clock = 0, i = 0;
    bool simulation_completed = false;
    node_t ready_list = NULL, new_list = NULL, waiting_list = NULL, terminated = NULL, temp, node;
    node_t running = NULL;

	// Initialize variables for input files
    char *input_file;
    char *memory_schema_file;

	// Initialize flags
    int verbose;
    int scheduler_type;
    int using_memory_schema = 0;

	// Initialize main memory representation
	struct memory_partition main_memory[NUM_MEMORY_PARTITIONS]; 

	if (argc == 6) {
        input_file = argv[1];
        scheduler_type = atoi(argv[2]);
        memory_schema_file = argv[3];
        using_memory_schema = atoi(argv[4]);
        verbose = atoi(argv[5]);
	} else if(argc == 5) {
        input_file = argv[1];
        scheduler_type = atoi(argv[2]);
        memory_schema_file = argv[3];
        using_memory_schema = atoi(argv[4]);
        verbose = 0;
    } else if(argc == 4){
        input_file = argv[1];
        scheduler_type = atoi(argv[2]);
        memory_schema_file = argv[3];
        using_memory_schema = atoi(argv[4]);
        verbose = 0;
    } else if( argc == 3 ) {
        input_file = argv[1];
        verbose = atoi(argv[2]);
    } else {
        printf("Three to six args expected. First int arg is Scheduler type argument (0 for FCFS, 1 for Priority, 2 for RR), add 1 after if verbose is desired \n");
        return -1;
    }


    // Read memory schema from file and build main memory
    if(verbose) printf("------------------------------- Building main memory------------------------------- \n");
    read_memory_schema_from_file(memory_schema_file, main_memory);
    if(verbose) for (i=0; i < NUM_MEMORY_PARTITIONS; i++) {
		printf("Partition %d size and is available (1 if true): %d Mb, %d\n", i, main_memory[i].size, main_memory[i].is_available);
	}
    if(verbose) printf("-------------------------------------------------------------------------------------\n");

    // Process metadata should be read from a text file
    if(verbose) printf("------------------------------- Loading all processes -------------------------------\n");
    new_list = read_proc_from_file(input_file);
    if(verbose) print_nodes(new_list);
    if(verbose) printf("-------------------------------------------------------------------------------------\n");
    if(verbose) printf("Starting simulation...\n");

    int time_quantum = 1;
    int current_quantum = 0;


    // print the headers
    printf("Time of transition,PID,Old State,New State\n");
    // Simulation loop
    do {
        // Update timers to reflect next simulation step
        // Advance all the io timers for processes in waiting state
        node = waiting_list;

        while(node != NULL){
            if (node ==NULL) break;
            node->p->io_time_remaining -= 1;
            if(node->p->io_time_remaining <= 0){
                // This process is ready, it should change states from waiting to ready
                // Update the time of next io event to the frequency of its occurance
                // add it to the ready queue and remove it from waiting list
                node->p->s = STATE_READY;
                node->p->io_time_remaining = node->p->io_frequency;

                temp = node->next;
                remove_node(&waiting_list, node);
                ready_list = push_node(ready_list, node);
                printf("%d,%d,%s,%s\n", cpu_clock, node->p->pid, STATES[STATE_WAITING], STATES[STATE_READY]);

                node = temp;
            } else {
                node = node->next;
            }
        }


        // Check if any of the items in new queue should be moved to the ready queue
        node = new_list;
        while (node != NULL) {
            // Check if process has arrived
            if (node->p->arrival_time <= cpu_clock) {
                bool memory_allocated = true;
                if (using_memory_schema) {
                    // Attempt to allocate memory to process
                    memory_allocated = allocate_memory_partition(&node, main_memory, verbose);
                }
                if(memory_allocated) {
					// Move process from NEW to READY
                    node->p->s = STATE_READY;
                    temp = node->next;
                    remove_node(&new_list, node);
                    ready_list = push_node(ready_list, node);
                    printf("%d,%d,%s,%s\n", cpu_clock, node->p->pid, STATES[STATE_NEW], STATES[STATE_READY]);
                    node = temp;
                } else {
					node = node->next; // NOTE(@braeden): This prevents an infinite loop when arrival_time <= clock && memory not allocated
				}
            } else {
                node = node->next;
            }
        }




        // Make sure the CPU is running a process
        if(running == NULL){
            // If it isn't, check if there is one ready
            if(ready_list!=NULL){
                if(scheduler_type == 0) { // FCFS
                    running = ready_list;
                } else if(scheduler_type == 1) { // External Priorities (No Preemption)
                    running = pick_highest_priority_process(&ready_list);
                } else if(scheduler_type == 2) { // Round Robin
                    running = ready_list; // Since it's just like FCFS but with a time slice.
                }
                running->p->s = STATE_RUNNING;
                remove_node(&ready_list, running);
                printf("%d,%d,%s,%s\n", cpu_clock, running->p->pid, STATES[STATE_READY], STATES[STATE_RUNNING]);
                current_quantum = 0;
            } else{
                running = NULL;
                if(verbose) printf("%d: CPU is idle\n", cpu_clock);
            }
        } else {
            // if it is then remove the time step from remaining time until process completetion and next io event
            running->p->cpu_time_remaining -= 1; 
            running->p->io_time_remaining -= 1; 
            // if(verbose) printf("%d: PID %d has %dms until completion and %dms until io block\n", cpu_clock,  running->p->pid, running->p->cpu_time_remaining,running->p->io_time_remaining);

            if(running->p->cpu_time_remaining <= 0){
                // The process is finished running, terminate it

                // Free up memory for another process 
				if(verbose) {printf("-------------------------------------------\n");}
				if(verbose) {printf("Freeing memory partition %d from process %d\n", running->p->index_of_memory_partition, running->p->pid);}
				if(verbose) {printf("-------------------------------------------\n");}
				main_memory[running->p->index_of_memory_partition].is_available = true;

                running->p->s = STATE_TERMINATED;
                terminated = push_node(terminated,running);
                printf("%d,%d,%s,%s\n", cpu_clock, running->p->pid, STATES[STATE_RUNNING], STATES[STATE_TERMINATED]);

                if(ready_list!=NULL){
                    running = ready_list;
                    running->p->s = STATE_RUNNING;
                    remove_node(&ready_list, running);
                    printf("%d,%d,%s,%s\n", cpu_clock, running->p->pid, STATES[STATE_READY], STATES[STATE_RUNNING]);

                } else{
                    running = NULL;
                    if(verbose) printf("%d: CPU is idle\n", cpu_clock);
                }

            } else if(running->p->io_time_remaining <= 0){
                // The process is blocked by io, update the timer and set state to waiting
                running->p->io_time_remaining = running->p->io_duration;
                running->p->s = STATE_WAITING;
                waiting_list = push_node(waiting_list,running);
                printf("%d,%d,%s,%s\n", cpu_clock, running->p->pid, STATES[STATE_RUNNING], STATES[STATE_WAITING]);

                if(ready_list!=NULL){
                    running = ready_list;
                    running->p->s = STATE_RUNNING;
                    remove_node(&ready_list, running);
                    printf("%d,%d,%s,%s\n", cpu_clock, running->p->pid, STATES[STATE_READY], STATES[STATE_RUNNING]);

                } else {
                    running = NULL;
                    if(verbose) printf("%d: CPU is idle\n", cpu_clock);
                }
            } else if(scheduler_type == 2 && current_quantum >= time_quantum) { // Handle Round Robin Logic
                // Round Robin time quantum exhaustion logic
                ready_list = push_node(ready_list, running);
                running->p->s = STATE_READY;
                if(ready_list != NULL) {
                    running = ready_list;
                    running->p->s = STATE_RUNNING;
                    remove_node(&ready_list, running);
                    printf("%d,%d,%s,%s\n", cpu_clock, running->p->pid, STATES[STATE_READY], STATES[STATE_RUNNING]);
                    current_quantum = 0;
                } else {
                    running = NULL;
                    if(verbose) printf("%d, CPU is idle\n", cpu_clock);
                }
            } else {
                current_quantum += 1;
            }
        }

        if(verbose){
            printf("-------------------------------------------------------------------------------------\n");
            printf("At CPU time %dms...\n", cpu_clock);
            printf("-------------------------------\n");
            printf("The CPU is currently running:\n");
            print_nodes(running);
            printf("-------------------------------\n");
            printf("The new process list is:\n");
            print_nodes(new_list);
            printf("-------------------------------\n");
            printf("The ready queue is:\n");
            print_nodes(ready_list);
            printf("-------------------------------\n");
            printf("The waiting list is:\n");
            print_nodes(waiting_list);
            printf("-------------------------------\n");
            printf("The terminated list is:\n");
            print_nodes(terminated);
            printf("-------------------------------------------------------------------------------------\n");
        }

        // Advance the cpu clock time
        cpu_clock += 1;

        // The simulation is completed when all the queues are empty, in otherwords, all programs have run to completion
        simulation_completed = (ready_list == NULL) && (new_list == NULL) && (waiting_list == NULL) && (running == NULL);
    } while(!simulation_completed);

    if(verbose) printf("-------------------------------------------------------------------------------------\n");
    if(verbose) printf("Simulation completed in %d ms.\n", cpu_clock);

    // The simulation is done, all the nodes are in the terminated list, free them
    clean_up(terminated);
}
