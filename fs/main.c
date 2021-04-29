// -------------------------
// Luke Weber & Jared Meade
// CptS 360 - Final Project
// -------------------------

// Project header
#include "project.h"

// Driver
int main(int argc, char *argv[])
{
    /// TODO Make all functions accept char **args, and take what we need from each one
//    int i, cmd;
//    char line[128], cname[64], pathname[128], parameter[128];
//    // Array of command functions
//    int (*fncns)(char *pathname) = {menu, make_dir, change_dir, pwd, list_dir, rmdir, creat_file, link, unlink, symlink, rm_file, chmod_file, chown_file, stat_file, touch_file, open_file, close_file, pfd, lseek_file, access_file, read_file, write_file, cat_file, cp_file, mv_file, mount, umount, cs, do_fork, do_ps, sync, quit};

    /// BEGIN
    printf("%s\n", seperator);

    /// INITIALIZE PROJECT
    printf(BOLDWHITE "\n[1] INITIATE STUFF\n" RESET);
    init();

    /// MOUNT ROOT
    printf(BOLDWHITE "\n[2] MOUNT ROOT\n" RESET);
    mount_root();

    /// TESTING
    printf(BOLDWHITE "\n[3] TESTING\n" RESET);

    // Mkdir/symlink
    list_dir("X");
    rm_file("X/tiny");
    creat_file("X/what");
    make_dir("X/dirtedir");
    list_dir("X");
    rm_file("X/what");
    list_dir("X");
    make_dir("X/test");
    rmdir("X/test");
    rmdir("X/y");
    list_dir("X");

    // Stat and touch
    creat_file("KAPOW!");
    stat_file("KAPOW!");
    touch_file("Xyzabc");
    touch_file("DaNkMeMeS");

    // Change mod
    chmod_file(0777, "DaNkMeMeS");
    change_dir("X");
    list_dir("");
    pwd();

    // Links
    change_dir("");
    creat_file("regularoldfile");
    link("regularoldfile", "X/linkfile");
    list_dir("");
    symlink("regularoldfile", "X/SOFTlinkfile");
    list_dir("X");



    /// MENU
    //menu();

    /*
    // Loop command input
    while (1)
    {
      // Currently running process
      printf("P%d running: ", running->pid);
      // Get user input
      printf("Input command: ");
      fgets(line, 128, stdin);
      // Kill the \r char at end
      line[strlen(line) - 1] = 0;
      if (line[0] == 0) continue;

      sscanf(line, "%s %s %64c", cname, pathname, parameter);

      cmd = findCmd(cname); // map cname to an index

      // Call function from array
      //fncns[cmd]();

      /* OLD SHIT
      switch(cmd)
      {
           // ------------ LEVEL 1 -------------------
           case 0 : menu();                   break;
           case 1 : make_dir();               break;
           case 2 : change_dir();             break;
           case 3 : pwd(cwd);                 break;
           case 4 : list_dir();               break;
           case 5 : rmdir();                  break;
           case 6 : creat_file();             break;
           case 7 : link();                   break;
           case 8 : unlink();                 break;
           case 9 : symlink();                break;
           case 10: rm_file();                break;
           case 11: chmod_file();             break;
           case 12: chown_file();             break;
           case 13: stat_file();              break;
           case 14: touch_file();             break;

           // -------------- LEVEL 2 ------------------
           case 20: open_file();              break;
           case 21: close_file();             break;
           case 22: pfd();                    break;
           case 23: lseek_file();             break;
           case 24: access_file();            break;
           case 25: read_file();              break;
           case 26: write_file();             break;
           case 27: cat_file();               break;
           case 28: cp_file();                break;
           case 29: mv_file();                break;

           // ------------- LEVEL 3 -------------------
           case 30: mount();                  break;
           case 31: umount();                 break;
           case 32: cs();                     break;
           case 33: do_fork();                break;
           case 34: do_ps();                  break;

           case 40: sync();                   break;
           case 41: quit();                   break;
           default: printf("invalid command\n");
                    break;
      }
    }*/

    // End separator
    printf("\n%s\n", seperator);

    return 0;
}
