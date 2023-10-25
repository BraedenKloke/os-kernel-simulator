#!/bin/sh
# 
# Build script for generating images using Mermaid.
#
# Resources:
# * mermaid: https://mermaid.js.org/
# * mermaid-cli: https://github.com/mermaid-js/mermaid-cli

# Assignment 2 Gantt Diagram of FCFS Algorithm without I/O
mmdc -i src/mermaid/assignment-2-gantt-fcfs-without-io.mmd \
	-o resources/images/assignment-2-gantt-fcfs-without-io.png

# Assignment 2 Gantt Diagram of RR Algorithm without I/O
mmdc -i src/mermaid/assignment-2-gantt-rr-without-io.mmd \
	-o resources/images/assignment-2-gantt-rr-without-io.png

# Assignment 2 Gantt Diagram of Multi-Level Feedback Queue
# without I/O
mmdc -i src/mermaid/assignment-2-gantt-mlfq-without-io.mmd \
	-o resources/images/assignment-2-gantt-mlfq-without-io.png
