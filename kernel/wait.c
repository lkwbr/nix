// Luke Weber, 11398889
// CptS 460, Lab #3
// Created 09/12/2016

PROC *kfork() 
{ 
        int i; 
        PROC *p = get_proc(&freeList); 
        if (!p) 
        { 
                printf("no more PROC, kfork() failed\n"); 
                return 0; 
        } 
        p->status = READY; 
        p->priority = 1; 
        p->ppid = running -> pid; 
        for (i = 1; i < 10; i++) 
        { 
                p->kstack[SSIZE - 1] = 0; 
        } 
        p->kstack[SSIZE - 1] = (int) body; 
        p->ksp = &p->kstack[SSIZE - 9]; 
        enqueue(&readyQueue, p); 
        return p; 
}
int ksleep(int event)
{
	running->event = event;
	running->status = SLEEP;
	tswitch();
}
int kwakeup(int event) 
{
	int i;
	PROC *p;
	for (i = 1; i < NPROC; i++)
	{
		p = &proc[i];
		if (p->status == SLEEP && p->event == event)
		{
			p->event = 0;
			p->status = READY;
			enqueue(&readyQueue, p);
		}
	}
}
int ready(PROC *p)
{
	p->status = READY;
	enqueue(&readyQueue, p);
}
int kexit(int exitValue)
{
	int i, wakeupP1 = 0;
	if (running->pid == 1 && nproc > 2)
	{
		printf("other procs still exist, P1 can't die yet\n");
		return -1;
	}
	for (i = 1; i < NPROC; i++)
	{
		p = &proc[i];
		if (p->status != FREE && p->ppid == running->pid)
		{
			p->pid = 1;
			p->parent = &proc[1];
			wakeupP1++;
		}
	}
	running->exitCode = exitValue;
	running->status = ZOMBIE;
	kwakeup(running->parent);
	if (wakeupP1)
	{
		kwakeup(&proc[0]);
	}	
	tswitch();
}
int kwait(int *status)
{
	PROC *p;
	int i, hasChild = 0;
	while (1)
	{
		for (i = 1; i < NPROC; i++)
		{
			p = &proc[i];
			if (p->status != FREE && p->ppid == running->pid)
			{
				hasChild = 1;
				if (p->status == ZOMBIE)
				{
					*status = p->exitCode;
					p->status = FREE;
					put_proc(&freeList, p);
					nproc--;
					return (p->pid);
				}
			}
		}
	}
	if (!hasChild) return -1;
	ksleep(running);
}
