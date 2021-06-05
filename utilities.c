#include <stdio.h>
#include <stdlib.h>

void error(char *file, int line_num, char *error)
{
    fprintf(stderr, "%s: %d error: %s\n", file, line_num, error);
}
