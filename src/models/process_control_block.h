#ifndef PROCESS_CONTROL_BLOCK_H
#define PROCESS_CONTROL_BLOCK_H

struct process_control_block {
    int pid;
    int arrival_time;
    int total_cpu_time;
    int io_frequency;
    int io_duration;
};

#endif
