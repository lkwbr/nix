int term, ser0, ser1;
int stdin, stdout;

# include "ucode.c" 

main(int argc, char *argv[])
{
	// Open /dev/tty0 as 0 (read) and 1 (write) in
	// order to display messages (e.g. printf,
	// using stdout)
	stdin = open("/dev/tty0", 0);
	stdout = open("/dev/tty0", 1);

	printf("==================================\n");	
	printf("LWINIT: Fork login task on console\n");
	printf("==================================\n");	

	// Start login processes: two serial, one normal
	login();	
}

int login()
{
	term = fork();
	if (term)
	{
		ser0 = fork();
		if (ser0)
		{
			ser1 = fork();
			if (ser1)
			{
				// Moderate login processes 
				parent();		
			}
			else
			{	
				// Start serial terminal 1
				//printf("Starting ttyS1 on pid = %d\n", getpid());
				exec("login /dev/ttyS1");
			}	
		}		
		else 
		{
			// Start serial terminal 0
			//printf("Starting ttyS0 on pid = %d\n", getpid());
			exec("login /dev/ttyS0");
		}

	}
	else
	{	
		// Run terminal
		//printf("Starting tty0 on pid = %d\n", getpid());
		exec("login /dev/tty0");
	}
}

int parent()
{
	int status, pid; 

	// Loop to log children back in once
	// they become killed (i.e. logged out)
	while(1)
	{
		printf("LWINIT: Waiting for a logout...\n\n");
		pid = wait(&status);
		if (pid == term)
		{
			// Fork new terminal 0 login process 
			term = fork();
			if (term) { continue; }
			else 
			{ 
				printf("Forking new tty0 on pid = %d\n", getpid());
				exec("login /dev/tty0"); 
				return; 
			}
		}
		else if (pid == ser0) 
		{
			// Fork new serial 0 login process 
			ser0 = fork();
			if (ser0) { continue; }
			else 
			{ 
				printf("Forking new ttyS0 on pid = %d\n", getpid());
				exec("login /dev/ttyS0"); 
				return;
			}
		}
		else if (pid == ser1)
		{
			// Fork new serial 1 login process 
			ser1 = fork();
			if (ser1) { continue; }
			else 
			{ 
				printf("Forking new ttyS1 on pid = %d\n", getpid());
				exec("login /dev/ttyS1"); 
				return; 
			}
		}
	}
}
