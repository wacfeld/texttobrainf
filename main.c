#include <stdio.h>
//#include <assert.h>
//#include <ctype.h>
//#include <errno.h>
//#include <float.h>
//#include <limits.h>
//#include <locale.h>
//#include <math.h>
//#include <setjmp.h>
//#include <signal.h>
//#include <stdarg.h>
//#include <stddef.h>
//#include <stdlib.h>
#include <string.h>
//#include <time.h>
#define putd(x) printf(#x ": %d\n", x)
#define MAXIN 10000
#define maxrange sizeof(rangefreq)/sizeof(int)

int rangefreq[] = {
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0
};
int rangeinds[] = {
    [32 ... 39] = 0,
    [40 ... 47] = 1,
    [48 ... 55] = 2,
    [56 ... 63] = 3,
    [64 ... 71] = 4,
    [72 ... 79] = 5,
    [80 ... 87] = 6,
    [88 ... 95] = 7,
    [96 ... 103] = 8,
    [104 ... 111] = 9,
    [112 ... 119] = 10,
    [120 ... 127] = 11
};

int rangeinits[] = { 4,5,6,7,8,9,10,11,12,13,14,15 };
int rangestarts[] = {32, 40, 48, 56, 64, 72, 80, 88, 96, 104, 112, 120};

int comprangeref[maxrange];
int curranges[maxrange];

char input[MAXIN];
char output[MAXIN];
int outi = 0;

void out(char c)
{
    output[outi++] = c;
}
void outstr(char *c)
{
    for(char *p = c; *p; p++)
        out(*p);
}

int main()
{
    printf("input: ");
    scanf("%[^\n]", input);

    if(!strlen(input))
        return 0;

    for(int i = 0; i < strlen(input); i++)
    {
        rangefreq[rangeinds[input[i]]]++;
    }

    int rangecount = 0;
    for(int i = 0; i < maxrange; i++)
    {
        if(rangefreq[i])
        {
            comprangeref[rangecount++] = i;
        }
    }

    printf("%d\n", outi);
    outstr("++++++++[-");

    for(int i = 0; i < rangecount; i++)
    {
        curranges[i] = rangestarts[comprangeref[i]];
        out('>');
        for(int j = 0; j < rangeinits[comprangeref[i]]; j++)
            out('+');
    }
    for(int j = 0; j < rangecount; j++)
        out('<');
    out(']');
    out('>'); // in cell 0
    int curcell = 0;

    for(int i = 0; i < strlen(input); i++)
    {
        // ascertain the range
        char c = input[i];
        int rawrange = rangeinds[c];
        int comprange;
        for(comprange = 0; comprangeref[comprange] != rawrange; comprange++) ;

        // navigate to cell
        int celldelta = comprange - curcell;
        if(celldelta > 0)
            for(int j = 0; j < celldelta; j++)
                out('>');
        else
            for(int j = 0; j < -celldelta; j++)
                out('<');
        curcell = comprange;

        // adjust value
        int valdelta = c - curranges[comprange];
        if(valdelta > 0)
            for(int j = 0; j < valdelta; j++)
                out('+');
        else
            for(int j = 0; j < -valdelta; j++)
                out('-');
        curranges[comprange] = c;

        // print & profit
        out('.');
    }

    printf("%d\n", outi);
    out(0);
    puts(output);

}
