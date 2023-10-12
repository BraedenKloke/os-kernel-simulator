#ifndef OUTPUT_SPOOL_ENTRY_H
#define OUTPUT_SPOOL_ENTRY_H

struct output_spool_entry {
    int time;
    int pid;
    char *old_state;
    char *new_state;
};

#endif
