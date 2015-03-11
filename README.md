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
### log2_bsr
Found an assembly function to do all my work for me.  Here is the one
line log2 in all of it's glory:

    asm(
        "bsr   %1, %0        ;"  // 

        :"=r"(result)         // output
        :"r"(x)               // input
        );



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
    x=0x0000: : 0
    x=0x0001: : 0
    x=0x0002: : 1
    x=0x0001: : 0
    x=0x0002: : 1
    x=0x0003: : 1
    x=0x0003: : 1
    x=0x0004: : 2
    x=0x0005: : 2
    x=0x0007: : 2
    x=0x0008: : 3
    x=0x0009: : 3
    x=0x000f: : 3
    x=0x0010: : 4
    x=0x0011: : 4
    x=0x001f: : 4
    x=0x0020: : 5
    x=0x0021: : 5
    x=0x003f: : 5
    x=0x0040: : 6
    x=0x0041: : 6
    x=0x007f: : 6
    x=0x0080: : 7
    x=0x0081: : 7
    x=0x00ff: : 7
    x=0x0100: : 8
    x=0x0101: : 8
    x=0x01ff: : 8
    x=0x0200: : 9
    x=0x0201: : 9
    x=0x03ff: : 9
    x=0x0400: : 10
    x=0x0401: : 10
    x=0x07ff: : 10
    x=0x0800: : 11
    x=0x0801: : 11
    x=0x0fff: : 11
    x=0x1000: : 12
    x=0x1001: : 12
    x=0x1fff: : 12
    x=0x2000: : 13
    x=0x2001: : 13
    x=0x3fff: : 13
    x=0x4000: : 14
    x=0x4001: : 14
    x=0x7fff: : 14
    x=0x8000: : 15
    x=0x8001: : 15
    x=0xfffe: : 15
    x=0xffff: : 15
    
    -----------------
    Performance Tests
    -----------------
    Running 1000000000 itterations of all algorithms:
         log2_bsr(): Real: 180  User / System: (179 / 0) 
         floor_log2_s_asm(): Real: 835  User / System: (834 / 0) 
         floor_log2_s_asm_loop(): Real: 1053  User / System: (1052 / 0) 
         floor_log2_s(): Real: 1188  User / System: (1187 / 0) 
         floor_log2_s_loop(): Real: 1046  User / System: (1045 / 0) 
         libmLog2(): Real: 3921  User / System: (3917 / 0) 
