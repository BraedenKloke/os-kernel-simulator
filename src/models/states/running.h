#ifndef RUNNING_H
#define RUNNING_H

struct running_state_table {
	struct process_control_block running_process;
	int running_time;
	int remaining_cpu_time;
	bool process_is_running;
};

#endif
