log2
=====

Fast log2() floor for finding magnitude of base 2 integers

Both C and inline assembly versions.

Usage
-----

make should build the test code.  (the functions are inline in the
header)

'make asm' will build the executiable, but will also dump out assembly
files, for tweaking.
codemonkey@monkeytree log2 $ ./test_log2 

Algorithms
-----------

### floor_log2_s
Our initial algorithm, in c:

    x |= (x >> 1);
    x |= (x >> 2);
    x |= (x >> 4);
    x |= (x >> 8);

    return (ones_s(x) -1);

### floor_log2_s_asm
The above algorithm, but written in assembly -- could use optimizing

### floor_log2_s_loop
Brute force, count the highest bit loop:

    unsigned short result=0;
    x = x >> 1;
    while (x != 0) {
        result++;
        x = x >> 1;
    }
    return result;

### floor_log2_s_asm_loop
Brute force loop, re-written in assembly.

### libmLog2 -- libm's log2()
Libm's log2 function (floating point -- very slow)

Performance / Unit Test Results
--------------------------------
The following results are from a test run on my machine, i7-970 @ 3.2Ghz
Note that the assembly implementation of floor_log2 is fastest.

(Code compiled with -O2 -- when I compile with O3, it notices that I'm
not really printing out the result, so it optimizes my calculations out)

    -----------------
    Unit Tests
    -----------------
    x=0x0000: asm()=0, asm_loop()=0, log2_s()=0 log2_s_loop()=0 libm:log2()=-inf
    x=0x0001: asm()=0, asm_loop()=0, log2_s()=0 log2_s_loop()=0 libm:log2()=0.000000
    x=0x0002: asm()=1, asm_loop()=1, log2_s()=1 log2_s_loop()=1 libm:log2()=1.000000
    x=0x0001: asm()=0, asm_loop()=0, log2_s()=0 log2_s_loop()=0 libm:log2()=0.000000
    x=0x0002: asm()=1, asm_loop()=1, log2_s()=1 log2_s_loop()=1 libm:log2()=1.000000
    x=0x0003: asm()=1, asm_loop()=1, log2_s()=1 log2_s_loop()=1 libm:log2()=1.584963
    x=0x0003: asm()=1, asm_loop()=1, log2_s()=1 log2_s_loop()=1 libm:log2()=1.584963
    x=0x0004: asm()=2, asm_loop()=2, log2_s()=2 log2_s_loop()=2 libm:log2()=2.000000
    x=0x0005: asm()=2, asm_loop()=2, log2_s()=2 log2_s_loop()=2 libm:log2()=2.321928
    x=0x0007: asm()=2, asm_loop()=2, log2_s()=2 log2_s_loop()=2 libm:log2()=2.807355
    x=0x0008: asm()=3, asm_loop()=3, log2_s()=3 log2_s_loop()=3 libm:log2()=3.000000
    x=0x0009: asm()=3, asm_loop()=3, log2_s()=3 log2_s_loop()=3 libm:log2()=3.169925
    x=0x000f: asm()=3, asm_loop()=3, log2_s()=3 log2_s_loop()=3 libm:log2()=3.906891
    x=0x0010: asm()=4, asm_loop()=4, log2_s()=4 log2_s_loop()=4 libm:log2()=4.000000
    x=0x0011: asm()=4, asm_loop()=4, log2_s()=4 log2_s_loop()=4 libm:log2()=4.087463
    x=0x001f: asm()=4, asm_loop()=4, log2_s()=4 log2_s_loop()=4 libm:log2()=4.954196
    x=0x0020: asm()=5, asm_loop()=5, log2_s()=5 log2_s_loop()=5 libm:log2()=5.000000
    x=0x0021: asm()=5, asm_loop()=5, log2_s()=5 log2_s_loop()=5 libm:log2()=5.044394
    x=0x003f: asm()=5, asm_loop()=5, log2_s()=5 log2_s_loop()=5 libm:log2()=5.977280
    x=0x0040: asm()=6, asm_loop()=6, log2_s()=6 log2_s_loop()=6 libm:log2()=6.000000
    x=0x0041: asm()=6, asm_loop()=6, log2_s()=6 log2_s_loop()=6 libm:log2()=6.022368
    x=0x007f: asm()=6, asm_loop()=6, log2_s()=6 log2_s_loop()=6 libm:log2()=6.988685
    x=0x0080: asm()=7, asm_loop()=7, log2_s()=7 log2_s_loop()=7 libm:log2()=7.000000
    x=0x0081: asm()=7, asm_loop()=7, log2_s()=7 log2_s_loop()=7 libm:log2()=7.011227
    x=0x00ff: asm()=7, asm_loop()=7, log2_s()=7 log2_s_loop()=7 libm:log2()=7.994353
    x=0x0100: asm()=8, asm_loop()=8, log2_s()=8 log2_s_loop()=8 libm:log2()=8.000000
    x=0x0101: asm()=8, asm_loop()=8, log2_s()=8 log2_s_loop()=8 libm:log2()=8.005625
    x=0x01ff: asm()=8, asm_loop()=8, log2_s()=8 log2_s_loop()=8 libm:log2()=8.997179
    x=0x0200: asm()=9, asm_loop()=9, log2_s()=9 log2_s_loop()=9 libm:log2()=9.000000
    x=0x0201: asm()=9, asm_loop()=9, log2_s()=9 log2_s_loop()=9 libm:log2()=9.002815
    x=0x03ff: asm()=9, asm_loop()=9, log2_s()=9 log2_s_loop()=9 libm:log2()=9.998590
    x=0x0400: asm()=10, asm_loop()=10, log2_s()=10 log2_s_loop()=10 libm:log2()=10.000000
    x=0x0401: asm()=10, asm_loop()=10, log2_s()=10 log2_s_loop()=10 libm:log2()=10.001408
    x=0x07ff: asm()=10, asm_loop()=10, log2_s()=10 log2_s_loop()=10 libm:log2()=10.999295
    x=0x0800: asm()=11, asm_loop()=11, log2_s()=11 log2_s_loop()=11 libm:log2()=11.000000
    x=0x0801: asm()=11, asm_loop()=11, log2_s()=11 log2_s_loop()=11 libm:log2()=11.000704
    x=0x0fff: asm()=11, asm_loop()=11, log2_s()=11 log2_s_loop()=11 libm:log2()=11.999648
    x=0x1000: asm()=12, asm_loop()=12, log2_s()=12 log2_s_loop()=12 libm:log2()=12.000000
    x=0x1001: asm()=12, asm_loop()=12, log2_s()=12 log2_s_loop()=12 libm:log2()=12.000352
    x=0x1fff: asm()=12, asm_loop()=12, log2_s()=12 log2_s_loop()=12 libm:log2()=12.999824
    x=0x2000: asm()=13, asm_loop()=13, log2_s()=13 log2_s_loop()=13 libm:log2()=13.000000
    x=0x2001: asm()=13, asm_loop()=13, log2_s()=13 log2_s_loop()=13 libm:log2()=13.000176
    x=0x3fff: asm()=13, asm_loop()=13, log2_s()=13 log2_s_loop()=13 libm:log2()=13.999912
    x=0x4000: asm()=14, asm_loop()=14, log2_s()=14 log2_s_loop()=14 libm:log2()=14.000000
    x=0x4001: asm()=14, asm_loop()=14, log2_s()=14 log2_s_loop()=14 libm:log2()=14.000088
    x=0x7fff: asm()=14, asm_loop()=14, log2_s()=14 log2_s_loop()=14 libm:log2()=14.999956
    x=0x8000: asm()=15, asm_loop()=15, log2_s()=15 log2_s_loop()=15 libm:log2()=15.000000
    x=0x8001: asm()=15, asm_loop()=15, log2_s()=15 log2_s_loop()=15 libm:log2()=15.000044
    x=0xfffe: asm()=15, asm_loop()=15, log2_s()=15 log2_s_loop()=15 libm:log2()=15.999956
    x=0xffff: asm()=15, asm_loop()=15, log2_s()=15 log2_s_loop()=15 libm:log2()=15.999978
    
    -----------------
    Performance Tests
    -----------------
    Running 1000000000 itterations of all algorithms:
         floor_log2_s_asm(): Real: 839  User / System: (838 / 0) 
         floor_log2_s_asm_loop(): Real: 1064  User / System: (1063 / 0) 
         floor_log2_s(): Real: 1193  User / System: (1192 / 0) 
         floor_log2_s_loop(): Real: 1055  User / System: (1053 / 0) 
         libmLog2(): Real: 3965  User / System: (3962 / 0) 
