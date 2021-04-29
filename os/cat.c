#include "ucode.c"

// Source: page 361 in KCW's book
main(int argc, char *argv[])
{
	STAT st_tty, st0, st1;
	int fd = 0, n = 0, lr = 0, rr = 0, i = 0;
	char buf[1024], c;
	char port[64];

	// Get current tty and populate STAT struct
	gettty(port);
	stat(port, &st_tty);

	// Get info about open FDs 0 and 1, and writes to STAT structs
	fstat(0, &st0); 
	fstat(1, &st1);

	// Check for redirection (pg. 367)	
	if (st_tty.st_dev != st0.st_dev || st_tty.st_ino != st0.st_ino) 
	{ lr = 1; } // 0 (stdin) has been redirected
	if (st_tty.st_dev != st1.st_dev || st_tty.st_ino != st1.st_ino) 
	{ rr = 1; } // 1 (stdout) has been redirected

	// If we've got a filename, open that for reading
	// (i.e. cat [filename])
	if (argv[1]) { fd = open(argv[1], O_RDONLY); }
	if (fd < 0) { printf("Open file '%s' failed\n", argv[1]); return; } // open file failed

	if (!fd) // fd == 0 (i.e. getting from stdin)
	{
		while (n = read(fd, &c, 1)) // one char at a time
		{
			if (c == '\r') { c = '\n'; }
			if (!lr)
			{	
				// No left redirection: get from stdin
				// Store data for later for fd1
				buf[i++] = c;
				if (c == '\n') { write(1, "\r", 1); } 
				//if (c == '\n') { write(2, "\r", 1); } 
				
				// Write a char to fd2
				write(1, &c, 1);
				//write(2, &c, 1);

				if (c == '\n')
				{
					// Return: write data to terminal
					write(1, buf, i - 1);
					//write(2, "\r", 1);
					write(1, "\r", 1);
					write(1, "\n", 1);
					i = 0;
				}
			
				if (c == EOF)
				{
					return 0;	
				}
			}
			else 
			{
				// Left redirection
				if (c == '\n' && !rr) { write(1, "\r", 1); } 
				//if (c == '\n' && !rr) { write(2, "\r", 1); } 
				write(1, &c, 1);
			}
		}
	}
	else
	{
		// Write to next [...] 
		while (n = read(fd, &c, 1))
		{
			// Don't write \r if we've got right redirection
			if (c == '\n' && !rr) { write(1, "\r", 1); } 
			//if (c == '\n' && !rr) { write(2, "\r", 1); } 
			write(1, &c, 1); // write to stdout
		}
	}
	close(fd);
}
