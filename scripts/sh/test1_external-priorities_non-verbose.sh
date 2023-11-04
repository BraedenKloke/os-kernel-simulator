#!/bin/sh
#
# Runs non-verbose test case 1 using External Priorities scheduling algorithm.

TEST_CASE_FILE="./resources/test/test_case_1.csv"
MEMORY_SCHEMA_FILE="./resources/schemas/memory_schema_1.csv"
OUTPUT_FILE="out/test/output_test1_external-priorities.csv"

./out/assignment2.o $TEST_CASE_FILE 1 $MEMORY_SCHEMA_FILE 0 > $OUTPUT_FILE
