#!/bin/sh
#
# Runs non-verbose test case 1 using FCFS scheduling algorithm and memory schema 1.

TEST_CASE_FILE="./resources/test/test_case_1.csv"
MEMORY_SCHEMA_FILE="./resources/schemas/memory_schema_1.csv"
OUTPUT_FILE="out/test/output_test1_memory1.csv"

./out/assignment2.o $TEST_CASE_FILE 0 $MEMORY_SCHEMA_FILE 1 > $OUTPUT_FILE
