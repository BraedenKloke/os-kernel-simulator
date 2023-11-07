#!/bin/sh
#
# Runs all non-verbose tests as a batch.

TESTS=(
	test1_fcfs_non-verbose.sh
	test2_fcfs_non-verbose.sh
	test1_external-priorities_non-verbose.sh
	test2_external-priorities_non-verbose.sh
	test1_rr_non-verbose.sh
	test2_rr_non-verbose.sh
	test1_memory1_non-verbose.sh
	test2_memory1_non-verbose.sh
	test1_memory2_non-verbose.sh
	test2_memory2_non-verbose.sh
)

for i in {0..9}
do
	echo "Running ${TESTS[i]} ..."
	./scripts/sh/"${TESTS[i]}"
done

echo "Tests complete."
