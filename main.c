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
#include <string.h>








void searchFS(const char *fname)
{
    Elf *e;
    int fd;
    DIR *dir;
    char *entry_path;
    char *symbol;
    struct dirent *entry;
    struct stat info;
    
    //fprintf(stderr, "%s\n", fname);
    stat(fname, &info);
    if(S_ISREG(info.st_mode)){
       if((fd = open(fname, O_RDONLY)) < 0){
           fprintf(stderr, "open(%s)\n", fname);
           perror("open failed");
           return;
       }
       if(!(e = open_elf(fd))){
          close(fd);
          return;
       } 
       while(symbol = next()){
           if(!find_dynsym(e, symbol)){
               break;
           }
       }
       if(!symbol){
           fprintf(stdout, "%s\n", fname);
       }
       close_elf(e);
       reset();
    }
    if(S_ISDIR(info.st_mode)){
        if(!(dir = opendir(fname))){
            fprintf(stderr, "opendir(%s)\n", fname);
            perror("opendir failed");
            return;
        }
        entry_path = malloc(0x1000);
        while(entry = readdir(dir)){
            if(!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")){
                continue;
            }
            memset(entry_path, 0, 0x1000);
            strcpy(entry_path, fname);
            strcat(entry_path, entry->d_name);
            if(entry->d_type == DT_DIR){
                strcat(entry_path, "/");
            }
            searchFS(entry_path);
        }
        free(entry_path);
        closedir(dir);
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
