#ifndef WAITING_H
#define WAITING_H

struct waiting_state_struct {
	struct process waiting_list[20];
	int waiting_time;
};

#endif
