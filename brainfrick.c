#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

#define TAPELEN         30000
#define LPSTACKLEN      3000

typedef int cell;

const char *prompt = ">> ";     /* repl prompt */
cell tape[TAPELEN];
const cell *fc = tape;          /* points to the first cell, required for tape bound checking */
cell *cp = tape;                /* points to the current cell */
char *lpstack[LPSTACKLEN];      /* loop pointer stack */
int lptop = -1;                 /* lpstack top element tracker */

void parse(char *);
void die(const char *errmsg);
char *f2str(char *fname);
void repl();
char *lastlp();
char *poplp();
void pushlp(char *);

int
main(int argc, char **argv)
{
        if (argc > 1) {
                char *code;
                code = f2str(argv[1]);
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
        fprintf(stderr,"%s", prompt);
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
        int input;

        instr_len = strlen(instr);
        while (instr_len--) {
                c = *instr;
                switch (c) {
                case '>':
                        /* moves data pointer to the next cell (rightwards) */
                        if (++cp > fc + TAPELEN)
                                die("tape memory out of bounds: overshot tape size");
                        break;
                case '<':
                        /* moves data pointer to the previous cell (leftwards) */
                        if (--cp < fc)
                                die("tape memory out of bounds: undershot tape size");
                        break;
                case '+':
                        /* increments value at cell pointed by cp */
                        ++(*cp);
                        break;
                case '-':
                        /* decrements value at cell pointed by cp */
                        --(*cp);
                        break;
                case '.':
                        /* output the byte at the data pointer */
                        putchar(*cp);
                        break;
                case ',':
                        /* accepts one byte of input and stores it in the current cell */
                        if ((input = getchar()) != '\n' && input != EOF)
                                *cp = input;
                        break;
                case '[':
                        /* loop begins */
                        if (!*cp)
                                /* jump to the end of the loop and break */
                                while (*(++instr) != ']')
                                        ;
                        else
                                pushlp(instr);
                        break;
                case ']':
                        if (*cp) {
                                /* add traversed length to instr_len to extend the while loop */
                                instr_len += (instr - lastlp());
                                /*
                                 * pop from loop pointer stack if current cell is 1, otherwise
                                 * just retrieve the last element without deleting it
                                 */
                                instr = *cp == 1 ? poplp() : lastlp();
                        }
                        break;
                default:
                        /* every other character is ignored */
                        break;
                }
                instr++;
        }
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
f2str(char *fname)
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
 * pushes cell pointed by cp to the loop pointer stack
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
