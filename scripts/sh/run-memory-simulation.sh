#!/bin/sh
#
# Runs a simulation to compare memory schema 1 against memory schema 2.

echo "Running memory simulation ..."

for i in {0..9}
do
	for j in {1..2}
	do
		echo "Running scenario ${i} with memory schema ${j}"
		scenario_file="./resources/memory-simulation/scenario${i}.csv"
		memory_schema_file="./resources/schemas/memory_schema_${j}.csv"
		output_file="./out/memory-simulation/scenario${i}_memory${j}.csv"
		./out/assignment2.o $scenario_file 0 $memory_schema_file 1 0 > $output_file
		cat $output_file
	done
done

echo "Memory simulation done."
