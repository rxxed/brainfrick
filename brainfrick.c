#include <stdio.h>
#include <stdlib.h>

#include "util.h"

#define TAPELEN 1000

typedef int cell;

cell tape[TAPELEN];
cell *dp = tape;               /* data pointer */

void next();
void prev();
void incr();
void decr();
void get();
void put();
void repl();
void parse(FILE *);

int
main(int argc, char **argv)
{
        if (argc > 1) {
                FILE *fp;
                fp = fopen(argv[1], "r");
                parse(fp);
        } else
                repl();


        /* ,. */
        /* get();put(); */

        /* ,>,.<. */
        /* get();next();get();put();prev();put(); */

        /* ,++-. */
        /* get();incr();incr();decr();put(); */

        return 0;
}

void
parse(FILE *fp)
{
        int c;
        while ((c = fgetc(fp)) != EOF) {
                switch (c) {
                case '>':
                        next();
                        break;
                case '<':
                        prev();
                        break;
                case '+':
                        incr();
                        break;
                case '-':
                        decr();
                        break;
                case '.':
                        put();
                        break;
                case ',':
                        get();
                        break;
                }
        }
}

void
repl()
{
        die("repl function not implemented");
}

/* accepts one byte of input and stores it in the current cell */
void
get()
{
        *dp = getchar();
}

/* output the byte at the data pointer */
void
put()
{
        putchar(*dp);
}

/* moves data pointer to the next cell (right) */
void
next()
{
        ++dp;
}

/* moves data pointer to the previous cell (left) */
void
prev()
{
        --dp;
}

/* increments value at cell pointed by dp */
void
incr()
{
        ++*dp;
}

/* decrements value at cell pointed by dp */
void
decr()
{
        --*dp;
}
