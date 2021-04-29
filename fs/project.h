// -------------------------
// Luke Weber, Jared Meade
// CptS 360 - Final Project
// -------------------------

// NOTE: Including a file is like pasting it's code at that point. For this reason, we have to
// be careful where we declare things.

// Include guards
#ifndef _PROJECTH_
#define _PROJECTH_

// Project libraries, types, and definitions
#include "util/types.h"

// Project globals
const int debugging_mode = 1;

PROC *running;
GD    *gp;
SUPER *sp;
INODE *ip;
DIR   *dp;
MINODE minode[NMINODES],
       *root = 0;
PROC procs[NPROC];
PROC *running;
char *cmdList[] = {"menu", "make_dir", "change_dir", "pwd", "list_dir", "rmdir", "creat_file", "link, unlink", "symlink", "rm_file", "chmod_file", "chown_file", "stat_file", "touch_file", "open_file", "close_file", "pfd", "lseek_file", "access_file", "read_file", "write_file", "cat_file", "cp_file", "mv_file", "mount", "umount", "cs", "do_fork", "do_ps", "sync", "quit"};
char *disk = "diskimage",
      *seperator = "--------------------------------------------------------";
char buf[BUFSIZ];

// Utilities
#include "util/alloc_dealloc.c"
#include "util/util.h"

// Level methods
#include "level1/level1.h"

#endif
