#ifndef READY_H
#define READY_H

struct ready_state_struct {
        struct process_control_block queue[1000];
        int first;
        int last;
        int queue_size;
};

#endif
