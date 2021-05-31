#include <stdio.h>
#include <stdlib.h>

void error(char *file, int line_num, char *error)
{
    printf("file: %s line number: %d error %s\n", file, line_num, error);
    exit(1);
}
