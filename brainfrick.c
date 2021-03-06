#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

#define TAPELEN         30000
#define LPSTACKLEN      3000
#define BF_INCR         '+'
#define BF_DECR         '-'
#define BF_NEXT         '>'
#define BF_PREV         '<'
#define BF_GET          ','
#define BF_PUT          '.'
#define BF_LOOPINIT     '['
#define BF_LOOPEND      ']'

typedef int cell;

const char *prompt = ">> ";     /* repl prompt */
cell tape[TAPELEN];
const cell *fc = tape;          /* points to the first cell, required for tape bound checking */
cell *dp = tape;                /* data pointer */
char *lpstack[LPSTACKLEN];      /* loop pointer stack */
int lptop = -1;                 /* loop stack top element tracker */

void next();
void prev();
void incr();
void decr();
void get();
void put();
void parse(char *);
void die(const char *errmsg);
char *ftostr(char *);
void repl();
char *lastlp();
char *poplp();
void pushlp(char *);

int
main(int argc, char **argv)
{
        if (argc > 1) {
                char *code;
                code = ftostr(argv[1]);
                parse(code);
        } else {
                repl();
        }

        return 0;
}

/*
 * switches to interactive mode
 */
void
repl()
{
        printf("%s", prompt);
        die("repl not implemented");
        /* char *line = readline(prompt); */
        /* if (!line) */
        /*         die("malloc error"); */
        /* parse(line); */
        /* free(line); */
}

void
parse(char *instr)
{
        int c;
        int instr_len;

        instr_len = strlen(instr);
        while (instr_len--) {
                c = *instr;
                switch (c) {
                case BF_NEXT:
                        next();
                        break;
                case BF_PREV:
                        prev();
                        break;
                case BF_INCR:
                        incr();
                        break;
                case BF_DECR:
                        decr();
                        break;
                case BF_PUT:
                        put();
                        break;
                case BF_GET:
                        get();
                        break;
                case BF_LOOPINIT:
                        if (!*dp)
                                /* jump to the end of the loop and break */
                                while (++*instr != BF_LOOPEND)
                                        ;
                        else
                                pushlp(instr);
                        break;
                case BF_LOOPEND:
                        if (*dp) {
                                /* add traversed length to instr_len to extend the while loop */
                                instr_len += (instr - lastlp());
                                /*
                                 * pop from loop pointer stack if current cell is 1, otherwise
                                 * just retrieve the last element without deleting it
                                 */
                                instr = *dp == 1 ? poplp() : lastlp();
                        }
                        break;
                }
                instr++;
        }
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

/* moves data pointer to the next cell (rightwards) */
void
next()
{
        if (++dp > fc + TAPELEN)
                die("tape memory out of bounds: overshot tape size");
}

/* moves data pointer to the previous cell (leftwards) */
void
prev()
{
        if (--dp < fc)
                die("tape memory out of bounds: undershot tape size");
}

/* increments value at cell pointed by dp */
void
incr()
{
        ++(*dp);
}

/* decrements value at cell pointed by dp */
void
decr()
{
        --(*dp);
}

/*
 * displays errmsg and exits the program
 */
void
die(const char *errmsg)
{
        fprintf(stderr, "%s", errmsg);
        if (errmsg[strlen(errmsg)-1] == ':') {
                fputc(' ', stderr);
                perror(NULL);
        } else {
                fputc('\n', stderr);
        }
        exit(1);
}

/*
 * returns contents of file 'fname' as a single string
 */
char *
ftostr(char *fname)
{
        FILE *fp;
        char *buffer = 0;
        long length;

        fp = fopen(fname, "r");
        if (!fp)
                die("couldn't open file:");
        fseek(fp, 0, SEEK_END);
        length = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        buffer = malloc(sizeof(char) * length);
        if (buffer)
                fread(buffer, 1, length, fp);
        fclose(fp);
        return buffer;
}

/*
 * pushes cell pointed by dp to the loop pointer stack
 */
void
pushlp(char *instr)
{
        if (lptop + 1 > LPSTACKLEN)
                die("loop pointer stack out of bounds");
        lpstack[++lptop] = instr;
}

/*
 * returns last element from the loop pointer stack without
 * deleting it
 */
char *
lastlp()
{
        if (lptop < 0)
                die("lastlp: no elements in loop pointer stack");
        return lpstack[lptop];
}

/*
 * pops last element from the loop pointer stack and returns it
 */
char *
poplp()
{
        if (lptop < 0)
                die("poplp: no elements in loop pointer stack");
        return lpstack[lptop--];
}
