#include "utilities.h"
#include <stdlib.h>
#include <string.h>

static struct {
    size_t size = 0;
    char *top = 0;
    char *index = 0;
}stack;

struct name{
    struct name *fwd;
    char buff[257];
}

void push(char *fname)
{
    struct name *new;    
    struct name *old;

    if((strlen(fname) == 0) || (strlen(fname) > 256)){
        error("stack.c", 21, "invalid name length.");
    }
    old = stack.top;
    new = malloc(sizeof(struct name));
    stack.top = new;
    new->fwd = old;
    strcpy(new->buff, fname);
}

void pop()
{
    struct name *old;

    if(!stack.top){
    
        return;
    }
    old = stack.top;
    stack.top = old->fwd;
    free(old);
}

char *next()
{






}

void rewind()
{
    stack.index = stack.top;
}

void cleanup()
{






}
