#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <sys/times.h>

#include "log2.h"

/*
 * Quick unit test.  Will assert if floor_log2_s() returns
 * a value different than floor(log2())
 */
void logTest(unsigned short x)
{
    unsigned short log_s;
    unsigned short log_asm;
    double log_f;

    log_s = floor_log2_s(x);
    log_asm = floor_log2_s_asm(x);
    log_f = log2((double)x);

    printf("x=0x%04x: floor_log2_asm()=%u, floor_log2_s()=%u log2()=%f\n",
            x, log_asm, log_s, log_f);

    // Cast should truncate the double into a floor()
    assert((log_asm == log_s) && (log_s == (unsigned short)log_f));
}

char *getTime(clock_t real_s, struct tms start,
    clock_t real_e, struct tms end)
{
    clock_t elapsed_r;
    struct tms elapsed;
    static char buffer[180];

    elapsed_r =  real_e - real_s;
    elapsed.tms_utime = end.tms_utime - start.tms_utime;
    elapsed.tms_stime = end.tms_stime - start.tms_stime;
    elapsed.tms_cutime = end.tms_cutime - start.tms_cutime;
    elapsed.tms_cstime = end.tms_cstime - start.tms_cstime;
    
    snprintf(buffer, sizeof(buffer), "Real: %u  User / System: (%u / %u) ", 
        (unsigned)elapsed_r, (unsigned)(elapsed.tms_utime + elapsed.tms_cutime),
        (unsigned)(elapsed.tms_stime + elapsed.tms_cstime));
    return buffer;
}

/*
 * Use times() to time repeated itterations of the log function
 */
unsigned short timeLog(unsigned long itterations)
{
    struct tms start, end;
    clock_t real_s, real_e;
    unsigned long i;
    unsigned short result=0;
    unsigned short value=0;

    real_s = times(&start);
    for (i = 0; i < itterations; i++) {
        value = (unsigned short)(i & 0xffff);
        result += floor_log2_s_asm(value);
    }
    real_e = times(&end);

    printf("%lu itterations of floor_log2_s_asm(): %s\n", 
        itterations, getTime(real_s, start, real_e, end));
    
    real_s = times(&start);
    for (i = 0; i < itterations; i++) {
        value = (unsigned short)(i & 0xffff);
        result += floor_log2_s(value);
    }
    real_e = times(&end);

    printf("%lu itterations of floor_log2_s(): %s\n", 
        itterations, getTime(real_s, start, real_e, end));
    
    real_s = times(&start);
    for (i = 0; i < itterations; i++) {
        value = (unsigned short)(i & 0xffff);
        result += log2(value);
    }
    real_e = times(&end);
    printf("%lu itterations of log2(): %s\n", 
        itterations, getTime(real_s, start, real_e, end));

    return result;
}

int main (int argc, char *argv[])
{
    unsigned short i;
    unsigned short x;

//    printf("log2_asm(%u) = %u\n", 0x1234, floor_log2_s_asm(0x1234));
//    printf("log2_asm(%u) = %u\n", 0x0, floor_log2_s_asm(0x0));
//    assert(0);

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

    // And, run some timings
    timeLog(100000000l);

    return 0;
}
