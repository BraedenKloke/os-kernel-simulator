#include "input.h"
#include "models/process_control_block.h"
#include <stdio.h>
#ifndef INPUT_C
#define INPUT_C
int read_from_file(char *file_name, struct process_control_block processes[]){

    FILE *file = fopen(file_name, "r"); 		// Open File

    if(file == NULL) 				// If File is non exsistant or empty return 0 and print error
    {
        printf("Error opening file or file is empty");
        return 0;
    }

    int num_processes = 0; // Initialize number of processes to 0 and use as array index


    // Scan file (assuming that our data is 5 integers seperated by spaces,
    // this will get messed up if we have text in the file, if so we need to skip a line),
    // Note that if we use a CSV file this function works the same,
    // arguments will just need to be seperated by commas
    // fscanf returns the number of sucsessfully matched and assigned input items, therefor it should return 5 or not proceed as something went wrong


    char c;  // Skip First line to avoid csv headers
    do {
        c = fgetc(file);
    } while (c != '\n');



    while ( fscanf (
            file,
            "%d, %d, %d, %d, %d",
            &processes[num_processes].pid,
            &processes[num_processes].arrival_time,
            &processes[num_processes].total_cpu_time,
            &processes[num_processes].io_frequency,
            &processes[num_processes].io_duration) == 5 ) {
        num_processes++;
    }


    fclose(file);
    return num_processes;
}

#endif
