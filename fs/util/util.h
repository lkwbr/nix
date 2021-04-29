#ifndef _UTILH_
#define _UTILH_

// Project header
#include "../project.h"

// Startup
int mount_root();
int init();

int do_have_permissions(MINODE *mip, int uid);

int is_reg_file(MINODE *mip);
int is_dir_file(MINODE *mip);

int tokenize(char path[], char returnval[][128], char *tok);

int get_block(int fd, int blk, char buf[]);
int put_block(int fd, int blk, char buf[]);
int tst_bit(char *buf, int bit);
int set_bit(char *buf, int bit);
int clr_bit(char *buf, int bit);

int getino(int fd, char *name);
int search(MINODE *mip, char *name);
MINODE *iget(int dev, int ino);
int iput(MINODE *mip);
int findmyname(MINODE *mip, int myino, char myname[]);
int findino(MINODE *mip, int *myino,int *parentino);
int print_dir_entries(MINODE *mip);

// Implementations
#include "util.c"

#endif
