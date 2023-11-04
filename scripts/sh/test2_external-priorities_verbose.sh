#!/bin/sh
#
# Runs verbose test case 2 using External Priorities scheduling algorithm.

TEST_CASE_FILE="./resources/test/test_case_2.csv"
MEMORY_SCHEMA_FILE="./resources/schemas/memory_schema_1.csv"
OUTPUT_FILE="out/test/output_test2_external-priorities.csv"

./out/assignment2.o $TEST_CASE_FILE 1 $MEMORY_SCHEMA_FILE 0 1
