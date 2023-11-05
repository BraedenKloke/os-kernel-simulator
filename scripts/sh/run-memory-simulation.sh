#!/bin/sh
#
# Runs a simulation to compare memory schema 1 against memory schema 2.

echo "Running memory simulation ..."

MEMORY_SCHEMA_1_FILE="./resources/schemas/memory_schema_1.csv"
MEMORY_SCHEMA_2_FILE="./resources/schemas/memory_schema_2.csv"
SCENARIOS_DIR="./resources/memory-simulation"
OUTPUT_DIR="./out/memory-simulation"

# Run Scenario 0
SCENARIO="scenario0"
echo "Running ${SCENARIO} ..."

echo $MEMORY_SCHEMA_1_FILE
echo $SCENARIOS_DIR
echo $OUTPUT_DIR
echo $SCENARIO
echo "${SCENARIOS_DIR}/${SCENARIO}.csv"
echo "${OUTPUT_DIR}/${SCENARIO}_memory1.csv"

./out/assignment2.o "${SCENARIOS_DIR}/${SCENARIO}.csv" 0 $MEMORY_SCHEMA_1_FILE 1  1 > "${OUTPUT_DIR}/${SCENARIO}_memory1.csv"
#./out/assignment2.o "${SCENARIOS_DIR}/${SCENARIO}.csv" 0 $MEMORY_SCHEMA_2_FILE 1 > "${OUTPUT_DIR}/${SCENARIO}_memory2.csv"

echo "Memory simulation done."
