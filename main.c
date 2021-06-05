#include "stack.h"
#include "elf.h"
#include "utilities.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>








void searchFS(const char *fname)
{
    Elf *e;
    DIR *d;
    int fd;
    char *symbol;
    char *fpath;
    struct stat fileInfo;
    struct dirent *entry;

    if((fd = open(fname, O_RDONLY)) < 0){
        perror(fname);
        return;
    }
    fstat(fd, &fileInfo);
    if(S_ISREG(fileInfo.st_mode)){
       if(!(e = open_elf(fd))){
          close(fd);
          return;
       } 
       while(symbol = next()){
           if(!find_dynsym(e, symbol)){
               close_elf(e);
               break;
           }
       }
       if(!symbol){
           close_elf(e);
           fprintf(stdout, "%s\n", fname);
       }
    }
    if(S_ISDIR(fileInfo.st_mode)){
        if(!(d = fdopendir(fd))){
            perror("opendir failed");
            return;
        }
        while((entry = readdir(d))){
            if(!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")){
                continue;
            }
            searchFS(entry->d_name);
        }
    }       
}

char *newroot = NULL;
int main(int argc, char *argv[])
{
    int c;

    while((c = getopt(argc, argv, "r:s:")) > 0){
        switch(c){
            case 'r':
                newroot = optarg;
                break;
            case 's':
                push(optarg);
                break;
            default:
                fprintf(stderr, "Usage: elfSearch -s name ... [-r newroot]\n");
                exit(1);
        }
    }
    if(newroot){
        if(chroot(newroot) < 0){
            perror("chroot failed.");
            exit(1);
        }
    }
    searchFS("/");
    cleanup();
}
