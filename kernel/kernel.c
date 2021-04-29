// Luke Weber, 11398889
// CptS 460, Lab #3
// Created 09/12/2016

int do_tswitch()
{
	tswitch();
}
int do_kfork()
{
	PROC *p = kfork();
	if (p == 0)
	{
		printf("kfork failed\n");
	}
	printf("PROC %d kfork a child %d\n", running->pid, p->pid);
	return p->pid;
}
int do_exit()
{
	kexit(0);
}
int do_stop()
{
	// TODO
	running->status = STOP;
	tswitch();
}
int do_continue()
{
	// TODO
	int pid, i;
	printf("input pid ");
	pid = geti();
	if (pid < 0 || pid >= NPROC)
	{
		printf("incorrect pid\n");
		return -1;
	}
	for (i = 0; i < NPROC; i++)
	{
		if (proc[i].pid == pid && proc[i].status == STOP)
		{
			proc[i].status = READY;
			enqueue(&readyQueue, proc[i]);
		} 
	}
}
int do_sleep()
{
	// TODO
	int event;
	printf("input event ");
	event = geti();
	ksleep(event);
}
int do_wakeup()
{
	// TODO
	int event;
	printf("input event ");
	event = geti();
	kwakeup(event);	
}
int reschedule()
{
	PROC *p, *tempQ = 0;
	while ((p = dequeue(&readyQueue)))
	{
		enqueue(&tempQ, p);
	}
	readyQueue = tempQ;
	rFlag = 0;
	if (running->priority < readyQueue->priority)
	{
		rFlag = 1;
	}
}
int chpriority(int pid, int pri)
{
	PROC *p; 
	int i, ok = 0, reQ = 0;
	if (pid == running->pid)
	{
		running->priority = pri;
		if (pri < readyQueue->priority)
		{
			rFlag = 1;
		}	
	return 1;
	}
	for (i = 1; i < NPROC; i++)
	{
		p = &proc[i];
		if (p->pid == pid && p->status != FREE)
		{
			p->priority = pri;
			ok = 1;
			if (p->status == READY)
			{
				reQ = 1;
			}
		}
	}
	if (!ok)
	{
		printf("chpriority failed\n");
		return -1;
	}
	if (reQ)
	{
		reschedule(p);
	}
}
int do_chpriority()
{
	int pid, pri;
	printf("input pid ");
	pid = geti();
	printf("input new priority ");
	pri = geti();
	if (pri < 1) pri = 1;
	chpriority(pid, pri);
}
int body()
{
	char c;
	while (1)
	{
		if (rflag)
		{
			printf("proc %d: reschedule\n", running->pid);
			rflag = 0;
			tswitch();		
		}
	printList("freeList ", freeList);
	printQ("readyQueue", readyQueue);
	printf("proc%d running: priority=%d parent=%d enter a char [s|f|t|c|z|a|p|w|q]: ", running->pid, running->priority, running->parent->pid);
	c = getc(); printf("%c\n", c);
	switch(c)
	{
		case 's': do_tswitch(); break;
		case 'f': do_kfork(); break;
		case 'q': do_exit(); break;
		case 't': do_stop(); break;
		case 'c': do_continue(); break;
		case 'z': do_sleep(); break;
		case 'a': do_wakeup(); break;
		case 'p': do_chpriority(); break;
		case 'w': do_wait(); break;
		default: printf("invalid command\n"); break;
	}
	}
}
