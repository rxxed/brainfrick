#include <stdio.h>
#include <stdlib.h>

#include "util.h"

void
die(const char *errmsg)
{
        printf("%s", errmsg);
        exit(1);
}
