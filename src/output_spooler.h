#ifndef OUTPUT_SPOOLER_H
#define OUTPUT_SPOOLER_H

#include "models/output_spool_entry.h"

static struct output_spool_entry output_spool[1000];
static int output_spool_size;

struct output_spool_entry create_output_spool_entry(
        int time,
        int pid,
        char old_state,
        char new_state
);

int write_to_output_spool(
	struct output_spool_entry data, 
	struct output_spool_entry output_spool[],
	int size
);

int write_output_spool_to_file(struct output_spool_entry output_spool[], char *output_file_name, int size);

#endif
