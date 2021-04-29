// Luke Weber, 11398889
// CptS 460, Lab #3
// Created 09/12/2016

// All code based on p.120 in textbook

#define NPROC 9
#define SSIZE 1024
#define FREE 0
#define READY 1
#define RUNNING 2
#define STOPPED 3
#define SLEEP 4
#define ZOMBIE 5
typedef struct proc {
	struct proc *next;
	int *ksp;
	int pid;
	int status;
	int ppid;
	struct proc *parent;
	int priority;
	int event;
	int exitCode;
	int kstack[SSIZE];
} PROC;
