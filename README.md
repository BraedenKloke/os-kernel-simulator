# SYSC 4001 Assignment 2

===============================================================================================================

# Authors

* Grant Phillips
* Braeden Kloke

# Acknowledgements

We would like to acknowledge Ben Earle (BenEarle@cmail.carleton.ca) for providing a solution for Assignment 1
which Professor Wainer distributed and confirmed we could use as a starting point for Assignment 2.

# Changes Since Last Assignment

* Implemented FCFS, External Priorities and Round Robin scheduling algorithms
* Implemented memory management with fixed partitions

===============================================================================================================

# Table of Contents

1. [Build](#build)
2. [Test](#test)

# Build <a name="build"></a>

## MacOS / Linux

Run the following command to build the project:

```
./scripts/sh/build.sh
```

Confirm the following binary object is built:

```
out/assignment2.o
```

## Windows

Coming soon ...

# Test <a name="test"></a>

The following tests use the following resources:

* resources/test/test_case_1.csv
* resources/test/test_case_2.csv
* resources/schemas/memory_schema_1.csv: 500 Mb, 250 Mb, 150 Mb, 100 Mb
* resources/schemas/memory_schema_2.csv: 300 Mb, 300 Mb, 350 Mb, 50 Mb

All test results can be found in the following files:

* out/test/output_test1_fcfs.csv
* out/test/output_test2_fcfs.csv
* out/test/output_test1_external-priorities.csv
* out/test/output_test2_external-priorities.csv
* out/test/output_test1_rr.csv
* out/test/output_test2_rr.csv
* out/test/output_test1_memory1.csv
* out/test/output_test2_memory1.csv
* out/test/output_test1_memory2.csv
* out/test/output_test2_memory2.csv

## MacOS / Linux

NOTE: Verbose options for each script are available by editing 'non-verbose' to 'verbose' in the script filename.

Run the following commands to run the various tests:

### Test FCFS

```
./scripts/sh/test1_fcfs_non-verbose.sh
./scripts/sh/test2_fcfs_non-verbose.sh
```

### Test External Priorities

```
./scripts/sh/test1_external-priorities_non-verbose.sh
./scripts/sh/test2_external-priorities_non-verbose.sh
```

### Test Round Robin

```
./scripts/sh/test1_rr_non-verbose.sh
./scripts/sh/test2_rr_non-verbose.sh
```

### Test Memory Schema 1

```
./scripts/sh/test1_memory1_non-verbose.sh
./scripts/sh/test2_memory1_non-verbose.sh
```

### Test Memory Schema 2

```
./scripts/sh/test1_memory2_non-verbose.sh
./scripts/sh/test2_memory2_non-verbose.sh
```

## Windows

Coming soon ...

