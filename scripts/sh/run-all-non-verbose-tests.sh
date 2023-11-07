#!/bin/sh
#
# Runs all non-verbose tests as a batch.

TEST_SCRIPTS=(
	test1_fcfs_non-verbose.sh
)

for test in $TEST_SCRIPTS
do
	echo "Running ${test} ..."
	./scripts/sh/$test
done

echo "Tests complete."
