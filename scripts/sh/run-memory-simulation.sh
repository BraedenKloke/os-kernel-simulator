#!/bin/sh
#
# Runs a simulation to compare memory schema 1 against memory schema 2.

echo "Running memory simulation ..."

# Run Scenario 0

./out/assignment2.o ./resources/memory-simulation/scenario0.csv 0 ./resources/schemas/memory_schema_1.csv 1 1 

echo "Memory simulation done."
