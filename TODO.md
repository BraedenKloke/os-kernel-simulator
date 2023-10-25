# TODO

# Completed

# Work in Progress (MAX_WIP=1)
- [ ] (@braeden) docs(assignment questions): write-up completed questions and gantt charts
- [ ] (@braeden) chore(email prof): re: question d of assignment, clarification
	- scheduler to implement: 1. re: case presented in Part I. c.)
	- assumption: external priorities: add a priority field
- [ ] (@braeden) test: run profs code with sh scripts
- [ ] (@grant) test: run profs code with bat scripts
- [ ] (@braeden) chore: download sealion

# Backlog
- [ ] docs(readme): add running instructions
- [ ] fix(output): should create unique output files for each unique input
- [ ] chore: review assignment 1 solution given by prof
- [ ] chore: submit file
- [ ] test: compare termination times of assignment 1 solution against prof's assignment 1 solution
- [ ] sealion

## Schedulers to implement
- [ ] feat(scheduler): add scheduler function to encapsulate scheduling algorithms (include c file and header)
- [ ] feat(scheduler): first come first server alg
- [ ] feat(scheduler): external prioriites algorithm
- [ ] feat(scheduler): round robin alg
- [ ] (@grant) test: create bat scripts


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
