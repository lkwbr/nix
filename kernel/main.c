// Luke Weber, 11398889
// CptS 460, Lab #3
// Created 09/12/2016

#include "type.h"
PROC proc[NPROC], *running, *freeList, *readyQueue, *sleepList;
int procSize = sizeof(PROC);
int nproc, rflag;
#include "io.c"
#include "queue.c"
#include "wait.c"
#include "kernel.c"
int init()
{
	PROC *p;
	int i;
	for (i = 0; i < NPROC; i++)
	{
		p = &proc[i];
		p->pid = i;
		p->status = FREE;
		p->priority = 0;
		p->next = &proc[i+1];
	}
	freeList = &proc[0];
	proc[NPROC - 1].next = 0;
	readyQueue = sleepList = 0;
	p = get_proc(&freeList);
	p->status = READY;
	running = p;
	nproc = 1;
}
int scheduler()
{
	if (running->status == READY)
	{
		enqueue(&readyQueue, running);
	}
	running = dequeue(&readyQueue);
	rflag = 0;
}
int main()
{
	printf("MTX starts in main()\n");
	init();
	kfork();
	while (1)
	{
		while (!readyQueue);
		tswitch();
	}
}
