#!/bin/sh
#
# Runs verbose test case 1 using External Priorities scheduling algorithm.

TEST_CASE_FILE="./resources/test/test_case_1.csv"
MEMORY_SCHEMA_FILE="./resources/schemas/memory_schema_1.csv"

./out/assignment2.o $TEST_CASE_FILE 1 $MEMORY_SCHEMA_FILE 0 1
