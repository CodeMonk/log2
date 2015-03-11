#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <sys/times.h>

#include "log2.h"

typedef struct {
    clock_t    start_r;
    struct tms start;
    clock_t    end_r;
    struct tms end;
} Timer;
void startTimer(Timer *timer)
{
    timer->start_r = times(&timer->start);
}
void stopTimer(Timer *timer)
{
    timer->end_r = times(&timer->end);
}
Timer *getElapsed(Timer *timer, Timer *elapsed)
{
    elapsed->start_r =  timer->end_r - timer->start_r;
    elapsed->start.tms_utime = timer->end.tms_utime - timer->start.tms_utime;
    elapsed->start.tms_stime = timer->end.tms_stime - timer->start.tms_stime;
    elapsed->start.tms_cutime = timer->end.tms_cutime - timer->start.tms_cutime;
    elapsed->start.tms_cstime = timer->end.tms_cstime - timer->start.tms_cstime;
    
    // Fill out the end, for grins
    elapsed->end_r = elapsed->start_r;
    elapsed->end = elapsed->start;
    
    return elapsed;
}
char *displayElapsed(char *dest, size_t destLen, Timer *elapsed)
{
    
    snprintf(dest, destLen, "Real: %u  User / System: (%u / %u) ", 
        (unsigned)elapsed->start_r, 
        (unsigned)(elapsed->start.tms_utime + elapsed->start.tms_cutime),
        (unsigned)(elapsed->start.tms_stime + elapsed->start.tms_cstime));
    return dest;
}

/*
 * Quick unit test.  Will assert if floor_log2_s() returns
 * a value different than floor(log2())
 */
void logTest(unsigned short x)
{
    unsigned short log_s, log_loop, log_asm, log_asm_loop;
    double log_f;

    log_s = floor_log2_s(x);
    log_loop = floor_log2_s_loop(x);
    log_asm = floor_log2_s_asm(x);
    log_asm_loop = floor_log2_s_asm_loop(x);
    log_f = log2((double)x);

    printf("x=0x%04x: asm()=%u, asm_loop()=%u, log2_s()=%u log2_s_loop()=%u libm:log2()=%f\n",
            x, log_asm, log_asm_loop, log_s, log_loop, log_f);

    // Cast should truncate the double into a floor()
    assert((log_asm == log_asm_loop) && 
            (log_asm == log_s) &&
            (log_asm == log_loop) &&
            (log_s == (unsigned short)log_f));
}




unsigned short runTest(unsigned short (*func)(unsigned short), 
        unsigned long times, Timer *elapsed)
{
    Timer timer;
    unsigned short result=0;
    unsigned short value=0;

    startTimer(&timer);
    while (times > 0) {
        value = (unsigned short)(times & 0xffff);
        result += func(value);
        times--;
    }
    stopTimer(&timer);
    
    getElapsed(&timer, elapsed);

    // force result to be used, so we dont' get optimized out.
    return result;
}

unsigned short libmLog2(unsigned short value)
{
    return (unsigned short)log2((double)value);
}

#define TIMEIT(func,num,elapsed,dummy,buff) \
    dummy += runTest(func, num, elapsed);                          \
    printf("     %s(): %s\n",                        \
        #func, displayElapsed(buff, sizeof(buff), elapsed))

/*
 * Use times() to time repeated itterations of the log function
 */
unsigned short timeLog(unsigned long itterations)
{
    Timer elapsed;
    unsigned short dummy_result=0;
    char buffer[256];

    printf("Running %lu itterations of all algorithms:\n", itterations);

    TIMEIT(floor_log2_s_asm, itterations,&elapsed, dummy_result, buffer);
    TIMEIT(floor_log2_s_asm_loop, itterations,&elapsed, dummy_result, buffer);
    TIMEIT(floor_log2_s, itterations,&elapsed, dummy_result, buffer);
    TIMEIT(floor_log2_s_loop, itterations,&elapsed, dummy_result, buffer);
    TIMEIT(libmLog2, itterations,&elapsed, dummy_result, buffer);

    return dummy_result;
}

int main (int argc, char *argv[])
{
    unsigned short i;
    unsigned short x;

//    printf("log2_asm(%u) = %u\n", 0x1234, floor_log2_s_asm(0x1234));
//    printf("log2_asm(%u) = %u\n", 0x0, floor_log2_s_asm(0x0));
//    assert(0);
    printf("\n-----------------\n");
    printf("Unit Tests\n");
    printf("-----------------\n");

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

    // And, run some performance tests 
    printf("\n-----------------\n");
    printf("Performance Tests\n");
    printf("-----------------\n");
    timeLog(1000000000l);

    return 0;
}
