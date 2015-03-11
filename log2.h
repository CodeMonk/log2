#ifndef __LOG2_H
#define __LOG2_H
/*
 * Count the number of ones (set bits) in an unsigned short.
 *
 * Returns 0-15 (because: math)
 */
inline unsigned short ones_s(register unsigned short x)
{
    x -= ((x >> 1) & 0x5555);
    x = (((x >> 2) & 0x3333) + (x & 0x3333));
    x = (((x >> 4) +x) & (0x0f0f));
    x += (x >> 8);
    //x += (x >> 16);
    return (x & 0x3f);
}

/*
 * Return floor(log2()) of the supplied short.
 * Short circuits on undefined (zero)
 */
inline unsigned short floor_log2_s(register unsigned short x)
{
    // Short circuit on invalid.
    if (x == 0)
        return 0;

    x |= (x >> 1);
    x |= (x >> 2);
    x |= (x >> 4);
    x |= (x >> 8);

    return (ones_s(x) -1);
}

/*
 * And, same logic, but in assembly.
 */
inline unsigned short floor_log2_s_asm(register unsigned short x)
{
    unsigned short result=0;
    asm(
        "xorl   %%eax, %%eax  ;"  // Move X to whole long
        "movw   %1, %%ax      ;"  // 

        "testw  %%ax, %%ax    ;"
        "je     end_%=        ;"  // Short circuit zero

        "movl   %%eax, %%edx  ;" // Move x to our dx
        "shrw   %%dx          ;" // x >> 1
        "orw    %%dx, %%ax    ;" // ax |= x >> 1

        "movw   %%ax, %%dx    ;" // dx = x
        "shrw   $2, %%dx      ;" // x >> 2
        "orw    %%dx, %%ax    ;" // ax |= x >> 2

        "movw   %%ax, %%dx    ;" // dx = x
        "shrw   $4, %%dx      ;" // x >> 4
        "orw    %%dx, %%ax    ;" // ax |= x >> 4

        "movw   %%ax, %%dx    ;" // dx = x
        "shrw   $8, %%dx      ;" // x >> 8
        "orw    %%dx, %%ax    ;" // ax |= x >> 8

        // Result now in ax, count the ones.
        //
        //    x -= ((x >> 1) & 0x5555);
        "movw   %%ax, %%dx    ;" // dx = x
        "shrw   %%dx          ;" // x >> 1
        "andw   $0x5555, %%dx ;" // &= 0x5555
        "subw   %%dx, %%ax    ;" // x -=


        //    x = (((x >> 2) & 0x3333) + (x & 0x3333));
        "movw   %%ax, %%dx    ;" // dx = x
        "andw   $0x3333, %%dx ;" // dx &= 0x3333
        "shrw   $2, %%ax      ;" // ax >> 2
        "andw   $0x3333, %%ax ;" // ax &= 0x3333
        "addw   %%dx, %%ax    ;" // Store the sum back in ax
        
        //    x = (((x >> 4) +x) & (0x0f0f));
        "movw   %%ax, %%dx    ;" // dx = x
        "shrw   $4, %%dx      ;" // dx >> 4
        "addw   %%dx, %%ax    ;" // ax += dx
        "andw   $0x0f0f, %%ax ;" // ax &= 0x0f0f

        //    x += (x >> 8);
        "movw   %%ax, %%dx    ;" // dx = x
        "shrw   $8, %%dx      ;" // dx >> 8
        "addw   %%dx, %%ax    ;" // ax += dx

        //    return (x & 0x3f);
        "andw   $0x3f, %%ax   ;" // ax &= 0x3f

        //    Finally, sub1
        "andw   $0x3f, %%ax   ;" // ax &= 0x3f
        "subw   $0x1, %%ax    ;" // ax -= 1

        "end_%=:                 ;"
        "mov    %%ax, %0      ;"

        :"=r"(result)         // output
        :"r"(x)               // input
        :"%eax", "%edx"       // clobbered registers
        );

    return result;
}

/*
 * Looping logic this time
 */
inline unsigned short floor_log2_s_loop(register unsigned short x)
{
    unsigned short result=0;
    x = x >> 1;
    while (x != 0) {
        result++;
        x = x >> 1;
    }
    return result;
}

/*
 * Looping logic this time
 */
inline unsigned short floor_log2_s_asm_loop(register unsigned short x)
{
    unsigned short result=0;
    asm(
        //"xorl   %%eax, %%eax  ;"  // Try only using 16 bits
        "movw   %1, %%ax      ;"  // 

        "shrw   %%ax          ;"  // Test for zero or one
        "testw  %%ax, %%ax    ;"  // And, we ignore the first shift anyway
        "je     end_%=        ;"  // Short circuit zero

        "xorl   %%edx, %%edx  ;" // zero out our counter

        "loop_%=:             ;" // Loop until we hit zero
        "incw   %%dx          ;" // increment counter
        "shrw   %%ax          ;" // x >> 1
        "jne    loop_%=       ;" // And, loop if not zero

        "movw   %%dx, %%ax    ;" // Move our result (count) into ax

        "end_%=:              ;"
        "mov    %%ax, %0      ;"

        :"=r"(result)         // output
        :"r"(x)               // input
        :"%eax", "%edx"       // clobbered registers
        );

    return result;
}
#endif /* __LOG2_H */
