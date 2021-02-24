#include <stdio.h>

#define TAPELEN 1000

typedef int cell;

cell tape[TAPELEN];
cell *dp;               /* data pointer */
dp = &tape;

void next();
void prev();
void incr();
void decr();
void get();
void put();
void die();
void repl();

int
main(int argc, char **argv)
{
        FILE *fp;
        if (argc > 1) {
                fp = fopen(argv[1], 'r');
                if (!fp)
                        die("Error opening file");
        } else {
                repl();
        }
        parse(fp);
        return 0;
}

/* Moves data pointer to the next cell (right) */
void
next()
{
        dp++;
}

/* Moves data pointer to the previous cell (left) */
void
prev()
{
        dp--;
}

/* Increments value at cell pointed by dp */
void
incr()
{
        *dp++;
}

/* Decrements value at cell pointed by dp */
void
decr()
{
        *dp--;
}
