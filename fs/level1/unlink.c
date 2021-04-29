#include "../project.h"

// SOURCE: http://www.eecs.wsu.edu/~cs360/unlink.html
/*
(1). get pathname's INODE into memory

(2). verify it's a FILE (REG or LNK), can not be a DIR;

(3). decrement INODE's i_links_count by 1;

(4). if i_links_count == 0 ==> rm pathname by

        deallocate its data blocks by:

     Write a truncate(INODE) function, which deallocates ALL the data blocks
     of INODE. This is similar to printing the data blocks of INODE.

        deallocate its INODE;

(5). Remove childName=basename(pathname) from the parent directory by

        rm_child(parentInodePtr, childName)

     which is the SAME as that in rmdir or unlink file operations.
*/

int unlink(char *pathname)
{
    int dev, ino;
    MINODE *mip;
    char new_pathname_copy_1[256], new_pathname_copy_2[256], *new_dir_name, *new_base_name;

    // Pathname's inode into memory
    if (pathname[0] == '/') dev = root->dev;
    else dev = running->cwd->dev;
    ino = getino(dev, pathname);
    mip = iget(dev, ino);

    // Verify it's a file
    if (is_dir_file(mip)) return EXIT_FAILURE;

    // Decrement inode's i_link_counts by 1
    mip->INODE.i_links_count--;

    // Check if we should remove the file
    if (mip->INODE.i_links_count == 0)
    {
        /// TODO: Remove pathname (truncate file and remove from parent directory)

        printf(YELLOW "\t...UNLINK(%s)" RESET ": Unlinked File has no more references to it. Truncating file...\n",
           pathname);
        return EXIT_SUCCESS;
    }

    printf(YELLOW "\t...UNLINK(%s)" RESET ": Unlinked file still has references. Doing nothing.\n",
           pathname);

    return EXIT_SUCCESS;
}
