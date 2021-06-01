#include "utilities.h"
#include <stdlib.h>
#include <string.h>


struct name{
    struct name *fwd;
    char buff[257];
};

static struct name *top = 0;
static struct name *idx = 0;

void push(char *fname)
{
    struct name *new;    
    struct name *old;

    if((strlen(fname) == 0) || (strlen(fname) > 256)){
        error("c", 21, "invalid name length.");
    }
    old = top;
    new = malloc(sizeof(struct name));
    top = new;
    idx = new;
    new->fwd = old;
    strcpy(new->buff, fname);
}

void pop()
{
    struct name *old;

    old = top;
    if(!old){
    
        return;
    }
    top = old->fwd;
    idx = old->fwd;
    free(old);
}

char *next()
{
   char *str = 0; 

   if(idx){
       str = idx->buff;
       idx = idx->fwd;
   }
   return str;
}

void reset()
{
    idx = top;
}

void cleanup()
{
    while(top){
        pop();
    }
}
