#include "ucode.c"

char *tty;

main(int argc, char *argv[]) // invoked by login exec 
{
	char line[200], *pipe_tokens[75];
	int i, pid = -1, status = -1, pd[2], num_tokens;

	// Command loop
	printf("\n");
	while(1)
	{
		// User-supplied command line
		delay(1000000); // space from past output
		printf("::::::::::::::::::::::::::::::::::\n");	
		printf("LWSH> ");
		gets(line);

		// Skip empty lines & check for logout
		if (*line == 0) { continue; }
		if ((strcmp(line, "logout") == 0) || (strcmp(line, "exit") == 0))
		{ logout(); }	

		printf("Entered = '%s'\n", line);
		
		// Split-up pipes
		num_tokens = splitup_tokens(pipe_tokens, line, "|");

		// Fork a child to handle command line tokens
		pid = fork();
		if (pid)
		{
			// Login process: wait for command line to execute 
			wait(&status);
			printf("::: Command executed\n");
		}
		else
		{
			// Child: recursively evaluate all commands
			printf("Processing pipe...\n");
			process_pipe(pipe_tokens, 0, num_tokens);
		}

		// Clear buffers
		bzero(line);
		i = 0; while (pipe_tokens[i]) { bzero(pipe_tokens[i++]); }	
	}
}

int logout()
{
	// Notify parent that we've died, so parent
	// make fork another just like me!
	exit(0);
	return 0;
}

int delay(long it)
{
	while(it-- > 0);
	return 0;
}

int process_pipe(char **pipe_tokens, int i, int n)
{
	// Children are writers to their parents, and parents 
	// then become children to the next parent
	// e.g. 
	// 	[child 1] | [parent 1]
	//		  | [child 2 ] | [parent 2]
	//			       | [child 3 ] | ...

	int pd[2], status;

	// Base case
	if (i == n) { write(2, "Pipe has stopped!\n", 20); return 0; }

	// Create pipe	
	pipe(pd);
	if (fork()) 
	{ 
		// Parent (reader)
		close(pd[1]); // close writing
		dup2(pd[0], 0); // redirect stdin

		// Wait for child to write 
		wait(&status);

		// Recurse on next command
		printf("Pipe recursing...\n");
		process_pipe(pipe_tokens, i + 1, n);

		// NOTE: Without the exit(0), we have some issues
		exit(0);
	}
	else 
	{
		// Child (writer)
		close(pd[0]); // close reading

		// Only redirect stdout (to next command) if we are not last 
		// command in pipe; this way, we can display the very end of
		// the pipe on screen
		//if ((i + 1) != n) { dup2(pd[1], 1); }

		// Execute command: write to pipe	
		process_redirection(pipe_tokens[i]); 
		exit(0);
	}
}

int process_redirection(char *line)
{
	int num_tokens;
	char *redirect_tokens[50], line_cpy[100], *line_tokens[50];
	int i = 0, append = 0; 

	// Clear buffers
	while (redirect_tokens[i]) { bzero(redirect_tokens[i++]); }	

	// Out-file append (e.g. cat [filename] >> f2);
	// close stdout and open/create file for appending
	strcpy(line_cpy, line); // fresh line
	num_tokens = splitup_tokens(redirect_tokens, line_cpy, ">>");
	if (strstr(line, ">>"))
	{ 
		printf("Executing cmd = '%s' for >>\n", redirect_tokens[0]);
		close(1); 
		open(redirect_tokens[1], O_APPEND | O_RDWR | O_CREAT);
		append = 1;
		exec(redirect_tokens[0]);
	}	

	// Out-file write (e.g. cat [filename] > f2)
	// close stdout and open file for writing
	strcpy(line_cpy, line); // fresh line
	num_tokens = splitup_tokens(redirect_tokens, line_cpy, ">");
	if (num_tokens > 1 && !append) 
	{ 
		printf("Executing cmd = '%s' for >\n", redirect_tokens[0]);
		close(1); // close stdout 
		open(redirect_tokens[1], O_WRONLY | O_CREAT); // open file for write/create 
		// Execute the damn command
		exec(redirect_tokens[0]);
	}	

	// In-file read (e.g. a.out < infile);
	// close stdin and open file for reading
	strcpy(line_cpy, line); // fresh line
	num_tokens = splitup_tokens(redirect_tokens, line_cpy, "<");
	if (num_tokens > 1) 
	{ 
		close(0); // close stdin 
		open(redirect_tokens[1], O_RDONLY); // open file for read 
		exec(redirect_tokens[0]);
	}	

	// Execute regular commands, no redirection!	
	printf("Executing cmd = '%s' without redirection\n", redirect_tokens[0]);
	exec(redirect_tokens[0]);
}

char *trim(char *str)
{
	char *end;

	// Trim leading space
	while (is_space((unsigned char) *str)) 
	{ str++; }

	if (*str == 0)  // all spaces?
	{ return str; }

	// Trim trailing space
	end = str + strlen(str) - 1;
	while ((end > str) && is_space((unsigned char) *end)) 
	{ end--; }

	// Write new null terminator
	*(end + 1) = 0;

	return str;
}

int is_space(char c) 
{ 
	if (c == ' ') 
	{ return 1; } 
	else 
	{ return 0; }
}

int splitup_tokens(char **dest, char *str, char *tok)
{
	// NOTE: String copying was messing with tokenizing, so now
	// we're just altering the original str with strtok func	
	int di = 0;
	char *p;

	// Tokenize
	p = strtok(str, tok);
	dest[di++] = trim(p);
	while (p = strtok(0, tok))
	{ dest[di++] = trim(p); }
	dest[di] = 0; // terminate

	// Return number of tokens
	return di;	
}
