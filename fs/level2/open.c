#include "../project.h"


int open_file()
{
    char *pathname;
    int file_use_mode, dev, ino;
    MINODE *mip;

    // Get pathname and use mode
    printf(">> ENTER pathname: ");
    //.. pathname
    printf(">> ENTER use mode (0 = Read; 1 = Write; 2 = Read-write; 3 = Append): ");
    //.. file_use_mode

    // Get pathname's ino
    if(pathname[0] == '/') dev = root->dev;
    else dev = running->cwd->dev;
    ino = getino(dev, pathname);

    // Get its mip
    mip = iget(dev, ino);

    // Check if regular file
    is_reg_file(mip);

    // Check if we have the right permissions
    do_have_permissions(mip, mip->INODE.i_uid);

    print("\t...Good: File is regular and you have permissions.\n");

    // Check if file already opened for W, RW, APPEND : reject. Otherwise (R), accept
    /// TODO

    // Allocate free Open File Table (OFT)


    /*
      1. ask for a pathname and mode to open:
         You may use mode = 0|1|2|3 for R|W|RW|APPEND

      2. get pathname's inumber:
             if (pathname[0]=='/') dev = root->dev;          // root INODE's dev
             else                  dev = running->cwd->dev;
             ino = getino(&dev, pathname);  // NOTE: dev may change with mounting

      3. get its Minode pointer
             mip = iget(dev,ino);

      4. check mip->INODE.i_mode to verify it's a REGULAR file and permission OK.

         Check whether the file is ALREADY opened with INCOMPATIBLE mode:
               If it's already opened for W, RW, APPEND : reject.
               (that is, only multiple R are OK)

      5. allocate a FREE OpenFileTable (OFT) and fill in values:

             oftp->mode = mode;      // mode = 0|1|2|3 for R|W|RW|APPEND
             oftp->refCount = 1;
             oftp->minodePtr = mip;  // point at the file's minode[]

      6. Depending on the open mode 0|1|2|3, set the OFT's offset accordingly:

          switch(mode){
             case 0 : oftp->offset = 0;     // R: offset = 0
                      break;
             case 1 : truncate(mip);        // W: truncate file to 0 size
                      oftp->offset = 0;
                      break;
             case 2 : oftp->offset = 0;     // RW: do NOT truncate file
                      break;
             case 3 : oftp->offset =  mip->INODE.i_size;  // APPEND mode
                      break;
             default: printf("invalid mode\n");
                      return(-1);
          }

       7. find the SMALLEST i in running PROC's fd[ ] such that fd[i] is NULL
          Let running->fd[i] point at the OFT entry

       8. update INODE's time field
             for R: touch atime.
             for W|RW|APPEND mode : touch atime and mtime
          mark Minode[ ] dirty

       9. return i as the file descriptor
    */

    return EXIT_SUCCESS;
}
