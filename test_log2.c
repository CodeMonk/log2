#include <stdio.h>
#include <math.h>
#include <assert.h>

#include "log2.h"

/*
 * Quick unit test.  Will assert if floor_log2_s() returns
 * a value different than floor(log2())
 */
void logTest(unsigned short x)
{
    unsigned short log_s;
    double log_f;

    log_s = floor_log2_s(x);
    log_f = log2((double)x);

    printf("x=0x%04x -- floor_log2_s(%u)=%u log2(%u)=%f\n", x, 
        x, log_s, x,log_f);

    // Cast should truncate the double into a floor()
    assert(log_s == (unsigned short)log_f);
}


int main (int argc, char *argv[])
{
    unsigned short i;
    unsigned short x;

    for (i=0 ; i < 16; i++) {
        // for each power of 2, print our evil patterns
        x= 1 << i;
        // Check X, X-1, and X+1
        logTest(x-1);
        logTest(x);
        logTest(x+1);
    }

    // Finally, test -2 and -1
    logTest(0xfffe);
    logTest(0xffff);

    return 0;
}
