#!/bin/sh
#
# Runs verbose test case 1 using FCFS scheduling algorithm.

TEST_CASE_FILE="./resources/test/test_case_2.csv"
MEMORY_SCHEMA_FILE="./resources/schemas/memory_schema_1.csv"
OUTPUT_FILE="out/test/output_test2_fcfs.csv"

./out/assignment2.o $TEST_CASE_FILE 0 $MEMORY_SCHEMA_FILE 0 1
