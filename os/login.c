#include "ucode.c"

char *tty;

main(int argc, char *argv[]) // invoked by exec("login /dev/ttyXX")
{
	char username[50], password[50], *pgrm = 0, *home = 0;
	int is_user = 0, str_len = 0, uid = -1, gid = -1, pid = -1;
	tty = argv[1];

	// Login process may run on different terminals	
	close(0);
	close(1);
	close(2);

	// Open our own tty as stdin (0), stdout (1), 
	// and stderr(2)	
	open(tty, 0);
	open(tty, 1);
	open(tty, 2);

	// Store tty string in PROC.tty[] for putc()
	settty(tty);
	
	// Let parent (init) print first
	delay(500000);	

	// Now we may use printf, which calls putc()
	// to our tty	
	printf("LWLOGIN: Open %s as stdin, stdout, stderr\n", tty);

	// Ignore Ctrl+C interrupts so that Ctrl+C kills
	// other procs on this tty but not the main sh
	signal(2, 1);

	// Login loop
	while(1)
	{
		// Show login
		printf("----------------------------------\n");
		printf("Login: ");
		gets(username);
		printf("Password: ");
		gets(password);
		printf("----------------------------------\n");

		// Verify username and password
		printf("Attempting to login %s with %s\n", username, password);
		is_user = valid_credentials(username, password, &uid, &gid, &pgrm, &home); 
		if (is_user)
		{
			printf("\n");
			printf("----------------------------------\n");
			printf("WELCOME, %s!\n", username);	
	
			// Set processes UID (this user owns)
			pid = getpid();
			printf("...Setting process %d owner to %d\n", pid, uid);
			chuid(uid, gid);

			// Change to home directory
			printf("...Moving to home directory %s\n", home);
			chdir(home);

			// Execute program in users's account
			printf("...Executing default program %s\n", pgrm);
			exec(pgrm); // changes execution image
		}  
		printf("Sorry, try again\n"); 

		// Clear buffers (because Brian said so)
		// NOTE: It fucking worked
		//username[0] = 0;
		//password[0] = 0;
		bzero(username, 50);
		bzero(password, 50);
		bzero(pgrm, strlen(pgrm)); 
		bzero(pgrm, strlen(home)); 
	}
}

int valid_credentials(char* username, char* password, int *uid, int *gid, char **pgrm, char **home)
{
	char c, contents[400], buf[50], entry[100];
	char *tokens[50], *p = "/etc/passwd";
	int i, ei, ti, len = 0, n = 0, fd = open(p, 0); // read-only

	// Read file contents
	while ((n = read(fd, buf, sizeof(buf))) > 0)
	{
		for (i = 0; i < n; i++)
		{ contents[len + i] = buf[i]; }
		len += n;
	}
	contents[len] = 0; // terminate

	// Run through entries, verify user
	printf("\n");
	entry[0] = 0; ei = 0;
	for (i = 0; i < len; i++)
	{
		c = contents[i];
		if (c == '\n') 
		{ 
			// Got an entry: check it
			printf(" | %s\r\n", entry); 	
			entry[ei] = 0;
			
			// Entry tokens
			p = strtok(entry, ":"); ti = 0;
			tokens[ti++] = p; 	
			while(p = strtok(0, ":"))
			{
				tokens[ti++] = p; 	
			}

			// Check credentials
			if ((strcmp(tokens[0], username) == 0) && (strcmp(tokens[1], password) == 0))
			{
				*home = tokens[5];
				*pgrm = tokens[6];							
				*uid = atoi(tokens[3]);
				*gid = atoi(tokens[2]);
	 
				return 1; 
			}
	
			ei = 0;
		}
		else 
		{ entry[ei++] = c; }
	}
	printf("\n");
	
	// 1 for true, 0 for false
	return 0;
}

int delay(long it)
{ while (it-- > 0); return 0; }
