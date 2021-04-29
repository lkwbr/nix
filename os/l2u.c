#include "ucode.c"

int main(int argc, char *argv[])
{
	int fd1 = 0, fd2 = 1, n = 0, lr = 0, rr = 0;
	char port[64], c;
	STAT st_tty, st0, st1;

	printf("Running l2u...\n");

	gettty(port);
	stat(port, &st_tty);

	fstat(0, &st0); 
	fstat(1, &st1);

	if (st_tty.st_dev != st0.st_dev || st_tty.st_ino != st0.st_ino) { lr = 1; }
	if (st_tty.st_dev != st1.st_dev || st_tty.st_ino != st1.st_ino) { rr = 1; }

	if (argv[1]) { fd1 = open(argv[1], O_RDONLY); }
	if (argv[2]) { fd2 = open(argv[1], O_WRONLY|O_CREAT); }

	while (n = read(fd1, &c, 1))
	{
		if (!rr &&  c == '\n'&& fd2 == 1) { write(2, "\r", 1); }
		if (c <= 122 && 97 <= c) { c = c- 32; }
		write(fd2, &c, 1);
	}
	close(fd1);
	close(fd2);
}
