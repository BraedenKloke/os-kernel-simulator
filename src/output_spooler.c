#include <stdio.h>
#include "output_spooler.h"

struct output_spool_entry create_output_spool_entry(
	int time,
	int pid,
	char old_state,
	char new_state 
){
	struct output_spool_entry entry;

	entry.time = time;
	entry.pid = pid;
	entry.old_state = old_state;
	entry.new_state = new_state;

	return entry;	
};

int write_to_output_spool(
	struct output_spool_entry data, 
	struct output_spool_entry output_spool[],
	int size
){
	printf("INFO: Writing data to output spool ...\n");

	output_spool[size].time = data.time;
	output_spool[size].pid = data.pid;
	output_spool[size].old_state = data.old_state;
	output_spool[size].new_state = data.new_state;
	return 0;
};

// TODO(@braedenkloke): remove arg output_spool[], there is only one spool and the spooler should control it
int write_output_spool_to_file(struct output_spool_entry output_spool[], char *output_file_name) {
        printf("INFO: Writing output spool to file ...\n");

	FILE *output_file;
	output_file = fopen(output_file_name, "w+");
	fprintf(output_file, "Time, PID, Old State, New State\n");

        for(int i = 0; i < 20; i++) {
                fprintf(output_file, "%d, ", output_spool[i].time);
                fprintf(output_file, "%d, ", output_spool[i].pid);
                fprintf(output_file, "%c, ", output_spool[i].old_state);
                fprintf(output_file, "%c\n", output_spool[i].new_state);
        }

	fclose(output_file);	
        return 0;
}
