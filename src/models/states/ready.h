#ifndef READY_H
#define READY_H

struct ready_state_struct {
        struct process queue[20];
        int first;
        int last;
        int queue_size;
};

#endif
