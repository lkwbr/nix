#include "ucode.c"

// Macros for determining type of inode
#define	__S_ISTYPE(mode, mask)	(((mode) &  0170000) == (mask))
#define	S_ISDIR(mode)	 __S_ISTYPE((mode), 0040000)
#define	S_ISREG(mode)	 __S_ISTYPE((mode), 0100000)

// Declarations of all functions 
int cpy(char *src, char *dest);
int f2f(char *src, char *dest);
int d2d(char *src, char *dest);
int d2nd(char *src, char *dest);
int transfer(int fd1, int fd2);

main(int argc, char *argv[])
{
	char *src = argv[1], *dest = argv[2];
	
	// We need two files/dirs for cp
	// e.g. cp source dest 
	if (!argv[1] || !argv[2]) { return; }

	// Perform the copy
	// Cases:	
	//	- File to file (copy contents)
	// 	- File to dir (cd int dir, create file)
	//	- Dir to dir (copy all files in dir) 
	//	x Dir to file (invalid!)
	cpy(src, dest);
}

int cpy(char *src, char *dest)
{
	// Get info about two files/dirs 
	STAT path1, path2;
	stat(src, &path1);
	stat(dest, &path2);

	// File to file: if source is file, second must be file 
	if (S_ISREG(path1.st_mode)) { f2f(src, dest); }
	// Directory to directory
	else if (S_ISDIR(path1.st_mode) && S_ISDIR(path2.st_mode)) { d2d(src, dest); }
	// Directory to NEW directory 
	else if (S_ISDIR(path1.st_mode)) { d2nd(src, dest); }
	// Directory to file: invalid!
	else if (S_ISDIR(path1.st_mode) && S_ISREG(path2.st_mode)) { return; }
}

int transfer(int fd1, int fd2)
{
	int n = 0;
	char buf[1024];
	// Copy all contents from one file descriptor 
	// to another
	while (n = read(fd1, buf, sizeof(buf))) 
	{ write(fd2, buf, n); }
}

int f2f(char *src, char *dest)
{
	// File to file
	int fd1 = 0, fd2 = 0;
	char file[128], newfile[128], *p;
	STAT path;

	// Info about destination
	stat(dest, &path);

	if (S_ISDIR(path.st_mode))
	{
		// Destination is dir

		// Handle source 
		// TODO
		strcpy(file, src);
		p = strtok(file, " \n/");
		while (p)
		{
			bzero(newfile, sizeof(newfile));
			strcpy(newfile, p);
			p = strtok(0, " \n/");
		}

		// Handle destination 
		bzero(file);
		strcpy(file, dest);
		strcat(file, "/");
		strcat(file, newfile);
		dest = file;
	}

	// Open source for read, destination for writing/creating	
	fd1 = open(src, O_RDONLY);
	fd2 = open(dest, O_WRONLY | O_CREAT);
	
	// Transfer data and close up
	transfer(fd1, fd2);
	close(fd1); close(fd2);
}

int d2d(char *src, char *dest)
{
	// Dir to dir
	char recursivePath[128], ccd[128];
	char buf[1024], *cp;
	char dirEntry[64];
	int fd = 0, i = 0, n = 0, first = 1;
	DIR *dp;
	STAT path, path2;

	// Read from source	
	fd = open(src, O_RDONLY);
	while(n = read(fd, buf, sizeof(buf)))
	{
		cp = buf;
		dp = (DIR *) buf;

		if (first)
		{
			// Move to second dir
			cp += dp->rec_len; dp = (DIR *) cp;
			cp += dp->rec_len; dp = (DIR *) cp;
			first = 0;
		}

		while (cp < buf + sizeof(buf))
		{
			// Clear buffers	
			bzero(ccd, sizeof(ccd));
			bzero(recursivePath, sizeof(recursivePath));
			bzero(dirEntry, sizeof(dirEntry));

			// Get source dir entry path (i.e. [dir_name]/dir_entry)
			strncpy(dirEntry, dp->name, dp->name_len);
			strcpy(recursivePath, src);
			strcat(recursivePath, "/");
			strcat(recursivePath, dirEntry);

			// Destination dir entry path 
			strcpy(ccd, dest);
			strcat(ccd, "/");
			strcat(ccd, dirEntry);

			// TODO

			stat(recursivePath, &path);

			if (S_ISDIR(path.st_mode))
			{ 
				stat(ccd, &path2);
				if(!S_ISDIR(path2.st_mode)) { mkdir(ccd); }
			}

			// Recursively evaluate path
			cpy(recursivePath, ccd);
	
			// Next record!	
			cp += dp->rec_len;
			dp = (DIR *)cp;
		}
		bzero(buf, sizeof(buf));
	}
	close(fd);
}

int d2nd(char *src, char *dest)
{
	// Dir to new dir
	mkdir(dest);
	d2d(src, dest);
}
