#include "../project.h"

MINODE *change_dir(char *pathname)// this is the equivalent of cd. It sets the current working directy
{
    int ino, dev = running->cwd->dev;
    MINODE *mip = running->cwd;

    printf(YELLOW "\t...CHANGE_DIR(%s)" RESET ": Changed directory from '%s'",
       pathname, running->cwd->name);

    if (strlen(pathname) > 0)// find and set it to that i node if the name isnt empty
    {
        ino = getino(dev, pathname);//get the inum
        //printf("ino = %d", ino);
        mip = iget(dev, ino);// get the block
        // Put cwd back onto disk
        iput(running->cwd);
        // Change cwd to new minode
        running->cwd = mip;

        // Get name of new cwd
        strcpy(mip->name, pathname);
    }
    else// set to the root inode if name is empty
    {
        mip = iget(dev, 2);//get the root
        iput(running->cwd);//decrement refCount to cwd
        running->cwd = mip;
    }



    printf(" to '%s'\n", running->cwd->name);
}
