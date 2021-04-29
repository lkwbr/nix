#include "ucode.c"

main(int argc, char *argv[])
{
	int fd = 0, term = 0, n = 0, lr=0, rr = 0, i = 0, count = 0;
	char buf[1024], c, input;
	char port[64];
	STAT st_tty, st0, st1;
	
	// Get current term
	gettty(port);
	stat(port, &st_tty);

	// Get info about stdin/stdout
	fstat(0, &st0); 
	fstat(1, &st1);

	// Determine left- and right- redirection
	if (st_tty.st_dev != st0.st_dev || st_tty.st_ino != st0.st_ino) { lr = 1; }
	if (st_tty.st_dev != st1.st_dev || st_tty.st_ino != st1.st_ino) { rr = 1; }

	// If file passed, open file	
	if (argv[1]) { fd = open(argv[1], O_RDONLY); }
	// If no left redirection and no file, return
	if (!lr && !fd) { close(fd); return; }
	// If no right redirection, open current terminal stdin
	if (!rr) { term = open(port, O_RDONLY); }

	// Read from stdin or file 
	while (n = read(fd, &c, 1))
	{
		buf[i++] = c;
		if (c == '\n' && !rr) 
		{  
			if (count > 25) 
			{ 
				// Read max number of lines: await
				// user input	
				read(term, &input, 1); 
				if (input == ' ') { count = 0; } 
			}
		
			// Write whole line to screen	
			write(1, buf, i - 1);
			write(2, "\r", 1);
			write(1, "\n", 1);
			i = 0;
			count++;
		}
		else if (c == '\n')
		{
			// If no right redirection, don't
			// write the \r, rather just the \n
			write(1, buf, i);
			i = 0;
		}
	}

	close(fd);
	close(term);
}
