#include "../project.h"

// NOTE: int types are preferred to void types, according to KC

int pwd()//prints the current workign directory of the process.
{
    printf(YELLOW "\t...PWD()" RESET ": '%s'\n", running->cwd->name);
}
