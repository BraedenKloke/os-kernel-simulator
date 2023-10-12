#ifndef RUNNING_H
#define RUNNING_H

struct running_state_table {
	struct process_control_block process;
	bool process_is_running;
};

#endif
