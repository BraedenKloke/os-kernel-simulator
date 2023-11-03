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
    int io_time_remaining;
    enum STATE s;
};

// This structure represents a partition of main memory.
struct memory_partition {
    int size;
    bool is_available;
};

// This structure represents a main memory divided into fixed partitions.
struct main_memory {
    struct memory_partition partitions[NUM_MEMORY_PARTITIONS];
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
proc_t create_proc(int pid, int arrival_time, int total_cpu_time, int io_frequency, int io_duration, int priority){
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
    temp->io_time_remaining = io_frequency;
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
    int pid, arrival_time, total_cpu_time, io_frequency, io_duration, priority;

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

        // We create a process struct and pass it too create node, then add this node to the new_list
        proc = create_proc(pid, arrival_time, total_cpu_time, io_frequency, io_duration, priority);
        node = create_node(proc);
        new_list = push_node(new_list, node);

    }

    return new_list;
}

/* FUNCTION DESCRIPTION: read_memory_schema_from_file
* This function adds the memory partition to an array
* The parameters are:
*    - input_file: File name within project directory
*   - memory_partition_sizes: Array to store memory partition sizes
*
*/
void read_memory_schema_from_file(char *input_file, int *memory_partition_sizes){
    int MAXCHAR = 128;
    int count = 0;

    char row[MAXCHAR];
    //int memory_partition_sizes[NUM_MEMORY_PARTITIONS];

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
        memory_partition_sizes[count] = atoi(strtok(row, ","));
        count++;
    }
}

/* FUNCTION DESCRIPTION: construct_main_memory
* This function is a constructor for main memory
* The parameters are:
*   - mem: Main memory
*   - memory_partition_sizes: Array to store memory partition sizes
* There is no return value.
*/

void construct_main_memory(struct main_memory *mem, int* memory_partition_sizes) {
    int i;

    for (i = 0; i < NUM_MEMORY_PARTITIONS; i++) {
        mem->partitions[i].size = memory_partition_sizes[i];
        mem->partitions[i].is_available = true;
    }
}





/* FUNCTION DESCRIPTION: get_time_to_next_event
* This function returns the amount of simulation time until the next event occurs
* The parameters are:
*    - cpu_clock: Time since the start of the simulation
*    - running: The node containing the currently running process
*    - new queue: The list of precess that have yet to arrive in the cpu
*    - waiting_list: The list of processes that are waiting for io
* The return value is the time until the next event
*/
int get_time_to_next_event(int cpu_clock, node_t running, node_t new_list, node_t waiting_list){
    node_t temp;
    int next_exit=INT_MAX, next_block=INT_MAX, next_arrival=INT_MAX, next_io=INT_MAX;

    if(running != NULL){
        next_exit = running->p->cpu_time_remaining;
        next_block = running->p->io_time_remaining;
    }

    // Search the new queue for the time until its next event
    temp = new_list;
    while(temp != NULL){
        next_arrival = min(temp->p->arrival_time - cpu_clock, next_arrival);
        temp = temp->next;
    }

    // Search the waiting queue for the time until its next event
    temp = waiting_list;
    while(temp != NULL){
        next_io = min(temp->p->io_time_remaining, next_io);
        temp = temp->next;
    }

    return min(min(next_exit, next_block), min(next_arrival, next_io));
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
    int next_step = 0, cpu_clock = 0, i = 0;
    bool simulation_completed = false;
    node_t ready_list = NULL, new_list = NULL, waiting_list = NULL, terminated = NULL, temp, node;
    node_t running = NULL;

    struct main_memory mem;

    char *input_file;
    char *memory_schema_file;

    int verbose;
    int scheduler_type;
    int memory_partitions[NUM_MEMORY_PARTITIONS];

    if(argc == 4) {
        input_file = argv[1];
        scheduler_type = atoi(argv[2]);
        memory_schema_file = argv[3];
        verbose = 0;
    } else if(argc == 3){
        input_file = argv[1];
        verbose = 0;
    } else if( argc == 2 ) {
        input_file = argv[1];
        verbose = atoi(argv[2]);
    } else {
        printf("Two or three args expected. First int arg is Scheduler type argument (0 for FCFS, 1 for Priority, 2 for RR), add 1 after if verbose is desired \n");
        return -1;
    }

    // Read memory schema from file and build main memory
    if(verbose) printf("------------------------------- Building main memory------------------------------- \n");
    read_memory_schema_from_file(memory_schema_file,memory_partitions);
    construct_main_memory(&mem,memory_partitions);
    if(verbose) for (i=0; i < NUM_MEMORY_PARTITIONS; i++) {printf("Partition %d: %d Mb\n", i, mem.partitions[i].size);}
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
        // Advance the cpu clock time
        cpu_clock += next_step;
        // Advance all the io timers for processes in waiting state
        node = waiting_list;

        while(node != NULL){
            if (node ==NULL) break;
            node->p->io_time_remaining -= next_step;
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
        while(node!= NULL) {
            // If the program has arrived change its state and add it to ready queue
            if(node->p->arrival_time == cpu_clock){
                node->p->s = STATE_READY;

                temp = node->next;
                remove_node(&new_list, node);
                ready_list = push_node(ready_list, node);
                printf("%d,%d,%s,%s\n", cpu_clock, node->p->pid, STATES[STATE_NEW], STATES[STATE_READY]);

                node = temp;
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
            running->p->cpu_time_remaining -= next_step;
            running->p->io_time_remaining -= next_step;
            // if(verbose) printf("%d: PID %d has %dms until completion and %dms until io block\n", cpu_clock,  running->p->pid, running->p->cpu_time_remaining,running->p->io_time_remaining);

            if(running->p->cpu_time_remaining <= 0){
                // The process is finished running, terminate it
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
                current_quantum += next_step;
            }
        }

        // Set the simulation time advance
        next_step = get_time_to_next_event(cpu_clock, running, new_list, waiting_list);

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

        // The simulation is completed when all the queues are empty, in otherwords, all programs have run to completion
        simulation_completed = (ready_list == NULL) && (new_list == NULL) && (waiting_list == NULL) && (running == NULL);
    } while(!simulation_completed);
    if(verbose) printf("-------------------------------------------------------------------------------------\n");
    if(verbose) printf("Simulation completed in %d ms.\n", cpu_clock);

    // The simulation is done, all the nodes are in the terminated list, free them
    clean_up(terminated);
}