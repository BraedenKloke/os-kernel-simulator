#ifndef WAITING_H
#define WAITING_H

struct waiting_state {
	struct process_control_block process[1000];
	int wait_position;
};

#endif
