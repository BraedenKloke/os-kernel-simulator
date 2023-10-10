#ifndef WAITING_H
#define WAITING_H

struct waiting_state_struct {
	struct process_control_block waiting_list[20];
	int waiting_time;
};

#endif
