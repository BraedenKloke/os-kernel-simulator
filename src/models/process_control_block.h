#ifndef PROCESS_CONTROL_BLOCK_H
#define PROCESS_CONTROL_BLOCK_H

#include <stdbool.h>

struct process_control_block {
    int pid;
    int arrival_time;
    int total_cpu_time;
    int io_frequency;
    int io_duration;
	int waiting_time;
	int remaining_cpu_time;
	int running_time;
	bool is_waiting;
};

#endif
