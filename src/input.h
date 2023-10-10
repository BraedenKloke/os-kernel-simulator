#ifndef INPUT_H
#define INPUT_H

struct process {
    int pid;
    int arrival_time;
    int total_cpu_time;
    int io_frequency;
    int io_duration;
};

int read_from_file(char *file_name, struct process processes[]);

#endif
