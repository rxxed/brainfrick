#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

#define INLOOP          1
#define NOTINLOOP       0
#define TAPELEN         30000
#define LPSTACKLEN      3000
#define INCR            '+'
#define DECR            '-'
#define NEXT            '>'
#define PREV            '<'
#define GET             ','
#define PUT             '.'
#define LOOPBEG         '['
#define LOOPEND         ']'

typedef int cell;

const char *prompt = ">> ";     /* repl prompt */
int loopstatus = NOTINLOOP;
cell tape[TAPELEN];
cell *dp = tape;                /* data pointer */
cell *lpstack[LPSTACKLEN];      /* loop pointer stack */
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
cell *lastlp();
cell *poplp();
void pushlp();

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

/* switches to interactive mode */
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
        int len;
        cell *loop_ptr;

        len = strlen(instr);
        while (len--) {
                c = *instr;
                switch (c) {
                case NEXT:
                        next();
                        break;
                case PREV:
                        prev();
                        break;
                case INCR:
                        incr();
                        break;
                case DECR:
                        decr();
                        break;
                case PUT:
                        put();
                        break;
                case GET:
                        get();
                        break;
                case LOOPBEG:
                        if (!*dp) {
                                /* jump to the next ']' then break */
                                while (++*instr != ']')
                                        ;
                        } else {
                                pushlp();
                        }
                        break;
                case LOOPEND:
                        if (!*dp)
                                break;
                        if (lptop == 0)
                                loop_ptr = lastlp();
                        else
                                loop_ptr = poplp();
                        dp = loop_ptr;
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
        ++dp;
}

/* moves data pointer to the previous cell (leftwards) */
void
prev()
{
        --dp;
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

void
pushlp()
{
        if (lptop + 1 > LPSTACKLEN)
                die("loop pointer stack out of bounds");
        lpstack[++lptop] = dp;
}

cell *
lastlp()
{
        if (lptop < 0)
                die("no elements in loop pointer stack");
        return lpstack[lptop];
}

cell *
poplp()
{
        if (lptop < 0)
                die("no elements in loop pointer stack");
        return lpstack[lptop--];
}
