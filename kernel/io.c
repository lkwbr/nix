//#include <stdlib.h>
//#include <stdio.h>

typedef unsigned int u32;


/*
 * 	prints a character string
 *
 */
void prints(char *s)
{
	while(*s){
		putchar(*s);
		s++;
	}
}



char *table = "0123456789ABCDEF";

/*
 *
 *
 */
int rpu(u32 x, int BASE)
{
  char c;
  if (x){
     c = table[x % BASE];
     rpu(x / BASE, BASE);
     putchar(c);
  }
} 

/*
 *	prints an unsigned integer
 *
 */
int printu(u32 x)
{
  if (x==0)
     putchar('0');
  else
     rpu(x, 10);
  putchar(' ');
}

/*
 *	prints a decimal
 */
int printd(int x){
	if(x == 0)
		prints("0");
	else {
		if(x < 0){
			putchar('-');
			x = x & 0x7FFF;
		}
		rpu(x, 10);
	}

	prints(".0 ");
}

/*
 *	prints the given octal number
 */
int printo(u32 x){
	if(x == 0)
		prints("0");
	else {
		if(x < 0){
			putchar('-');
			x = x & 0x7FFF;
		}
		rpu(x, 8);
	}
}

/*
 *	prints the given hex number
 */
int printx(u32 x){
	if(x == 0)
		prints("0");
	else {
		if(x < 0){
			putchar('-');
			x = x & 0x7FFF;
		}
		rpu(x, 16);
	}
}




/*
 *	formatted print
 *  supports: 'c','s','u','d', 'o','x'
 */
void myprintf(char * fmt, ...){
	int * ip = &fmt + 1;
	char * cp = fmt;
	
	while(*cp){
		if(*cp != '%'){
			putchar(*cp);
			if(*cp == '\n')
				putchar('\r');
		}
		else{
			cp++;

			if(*cp == 'c'){
				putchar(*ip);
			}else if(*cp == 's'){
				prints(*ip);
			}else if(*cp == 'u'){
				printu(*ip);
			}else if(*cp == 'd'){
				printd(*ip);
			}else if(*cp == 'o'){
				printo(*ip);
			}else if(*cp == 'x'){
				printx(*ip);
			}

			ip++;
		}
		cp++;
	}
}
