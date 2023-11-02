# TODO

## Timeline
- Finish scheduler section by EOW Nov 3
- Finish memory management by EOW Nov 10

# Completed
- [X] (@braeden) docs(assignment questions): write-up completed questions and gantt charts
- [X] (@braeden) test: run profs code with sh scripts
- [X] (@braeden) refactor: merge new code base into main
- [X] (@braeden) chore: download sealion, use cmail
- [X] (@grant) test: create bat scripts
- [X] (@grant) test: run profs code with bat scripts

# Waiting to Hear Back
- [ ] (@braeden) chore(email prof): re: question d of assignment, clarification
	- scheduler to implement: 1. re: case presented in Part I. c.)
	- assumption: external priorities: add a priority field

# Work in Progress (MAX_WIP=1)

# Backlog
- [ ] (@braeden) docs(report): add Part D gantt chart to report
- [ ] (@grant) docs(readme): add build instructions for Windows
- [ ] (@braeden) docs(readme): add build instructions for Linux
- [ ] (@grant) docs(readme): add test instructions for Windows
- [ ] (@braeden) docs(readme): add test instructions for Linux
- [ ] (@braeden) docs(readme): add acknowledgements to README
- [ ] chore: submit assignment

## Schedulers to implement
- [ ] feat(scheduler): add scheduler function to encapsulate scheduling algorithms (include c file and header)
- [ ] feat(scheduler): first come first server alg
- [ ] feat(scheduler): external prioriites algorithm
- [ ] feat(scheduler): round robin alg



## Simulation execution
- [ ] (@grant) docs(report): submit report (3 pages max) analyzing three different scheudlers

## Memory Requirements
- [ ] feat(pcb): add info about (1) memory positions and (2) amount of memory needed
- [ ] feat: create memory management module
- [ ] test: run 10 different simulation scenarios, collect simulation results and analyze them
- [ ] docs(report): analyze results of mem management simulations

./main.exec fcfs input_file 
./main.exec extern_priorities input_file 
	- longest IO first?
	- shortest job first?
./main.exec rr input_file 
