#include "../project.h"

// Creates symbolic link from file described in new_pathname to file in old_pathname
int symlink(char *old_pathname, char *new_pathname)
{
    int dev, old_ino, new_ino, new_pino;
    MINODE *old_mip, *new_mip, *new_pmip;
    char old_pathname_copy[256], new_pathname_copy_1[256], new_pathname_copy_2[256],
        *old_base_name, *new_dir_name, *new_base_name;

    // Get running device
    if (old_pathname[0] == '/') dev = root->dev;
    else dev = running->cwd->dev;

    // Old pathname inode
    old_ino = getino(dev, old_pathname);
    old_mip = iget(dev, old_ino);

    // Verify old_pathname exists
    if (old_ino < 0) return EXIT_FAILURE;

    // Get old basename
    strcpy(old_pathname_copy, old_pathname);
    old_base_name = basename(old_pathname_copy);

    // Splitup new pathname
    strcpy(new_pathname_copy_1, new_pathname);
    strcpy(new_pathname_copy_2, new_pathname);
    new_dir_name = dirname(new_pathname_copy_1);
    new_base_name = basename(new_pathname_copy_2);

    // Does new parent exist and is a directory?
    new_pino = getino(dev, new_dir_name);
    if (new_pino < 0) return EXIT_FAILURE;
    new_pmip = iget(dev, new_pino);
    if (!is_dir_file(new_pmip)) return EXIT_FAILURE;

    // Does new file already exist?
    new_ino = getino(dev, new_pathname);
    if (new_ino >= 0) return EXIT_FAILURE;

    printf(YELLOW "\t...SYMLINK(%s, %s)" RESET ": <GOOD> New file parent dir exists, but new internal file does not\n",
            old_pathname, new_pathname);


    mysymfile(new_pmip, old_base_name, new_base_name);
    /// ^^^^^
    /// TODO: Create a file new_base_name in dir new_dir_name
    /// TODO: Change said file's type to LNK (0120000)
    /// TODO: Write the string from old_base_name into the i_block[] of file (which has room for 60 chars)
    /// TODO: Write the INODE of the new file back to disk

    /// Write back
//    new_mip->dirty = 1;
//    iput(new_mip);

//    new_pmip->dirty = 1;
//    iput(new_pmip);
//
//    old_mip->dirty = 1;
//    iput(old_mip);

    return EXIT_SUCCESS;
}


int mysymfile(MINODE *pip, char *old_name, char *name)// this is a helper function the algorithm is above
{
    char *cp;// traversing i nodes and dir.

    int ino, bno;
    char buf[BLKSIZE];

    // Allocate space for new inode under cwd
    ino = ialloc(running->cwd->dev);// allocated a new inode
    bno = balloc(running->cwd->dev);//allocated a block

    MINODE *mip = iget(running->cwd->dev, ino);// get the newly allocated inode

    ip = &mip->INODE;

    // FILE type mode
    ip->i_mode = 0120777; /// LNK mode?
    ip->i_uid  = running->uid;	// Owner uid
    ip->i_gid  = running->gid;	// Group Id
    ip->i_size = 0;		// Size in bytes
    ip->i_links_count = 0;
    ip->i_atime = ip->i_ctime = ip->i_mtime = time(0L);  // set to current time
    ip->i_blocks = 2;                	// LINUX: Blocks count in 512-byte chunks

    ip->i_block[0] = bno;

    // File has no data blocks
    int i = 0;
    for(i=0; i<15; i++) // set the iblocks
    {
        ip->i_block[i]=0;
    }

    /// Write string of old name into iblock
//    get_block(running->cwd->dev, ip->i_block[0], buf);
//    strcpy(buf, name);

    memcpy(&mip->INODE.i_block[0], name, strlen(name)+1);

    mip->dirty = 1;
    iput(mip);

    enter_name(pip, ino, name);

    pip->dirty = 1;
    iput(pip);
}
