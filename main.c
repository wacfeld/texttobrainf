#include <stdio.h>
#include <string.h>
#define putd(x) printf(#x ": %d\n", x)
#define MAXIN 100000
#define maxrange sizeof(rangefreq)/sizeof(int)

// frequency aspect is never used, only tested for nonzero (but a smarter person might be able to used frequency to further optimize)
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

// which range each printable ASCII code falls into
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

int rangeinits[] = { 4,5,6,7,8,9,10,11,12,13,14,15 }; // how much to multiply by 8, to init that range
int rangestarts[] = {32, 40, 48, 56, 64, 72, 80, 88, 96, 104, 112, 120}; // used to set curranges on init

int comprangeref[maxrange]; // maps the absolute range indices (0-11) to the compressed ones, because some aren't used
int curranges[maxrange]; // what value each brainF cell currently is at

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

    if(!strlen(input)) // don't bother with init at all, waste of time
        return 0;

    // figure out which ranges are used
    for(int i = 0; i < strlen(input); i++)
    {
        rangefreq[rangeinds[input[i]]]++;
    }

    // compress ranges (removed unused ones)
    int rangecount = 0;
    for(int i = 0; i < maxrange; i++)
    {
        if(rangefreq[i])
        {
            comprangeref[rangecount++] = i;
        }
    }

    // start with 8, multiply correspondingly to init. might be able to further optimize by making inits piggy back off each other, but that's too difficult.
    // maybe it would be possible to do a universal init that's actually pretty fast, idk. downside is that we don't get compression then
    //printf("%d\n", outi);
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

    // set up pointer
    out('>'); // in cell 0
    int curcell = 0;

    // produce brainF code
    for(int i = 0; i < strlen(input); i++)
    {
        //putd(outi);
        //putd(i);
        //putd(0);
        // ascertain the range
        char c = input[i];
        int rawrange = rangeinds[c];
        int comprange;
        //putd(c);
        //putd(rawrange);
        for(comprange = 0; comprangeref[comprange] != rawrange; comprange++) ;

        //putd(1);
        // navigate to cell
        int celldelta = comprange - curcell;
        if(celldelta > 0)
            for(int j = 0; j < celldelta; j++)
                out('>');
        else
            for(int j = 0; j < -celldelta; j++)
                out('<');
        curcell = comprange;
//putd(2);
        // adjust value
        int valdelta = c - curranges[comprange];
        if(valdelta > 0)
            for(int j = 0; j < valdelta; j++)
                out('+');
        else
            for(int j = 0; j < -valdelta; j++)
                out('-');
        curranges[comprange] = c;
//putd(3);
        // print & profit
        out('.');
    }

    // print number of chars used, and corresponding code
    printf("%d\n", outi);
    out(0);
    puts(output);

}
