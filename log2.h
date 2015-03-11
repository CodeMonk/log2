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
    x += (x >> 16);
    return (x & 0x3f);
}

/*
 * Return floor(log2()) of the supplied short.
 * Short circuits on undefined (zero)
 */
inline unsigned short floor_log2_s(register unsigned short x)
{
    // Short circuit on invalid.
    if (x == 0) return 0;

    x |= (x >> 1);
    x |= (x >> 2);
    x |= (x >> 4);
    x |= (x >> 8);
    
    return (ones_s(x) -1);
}
#endif /* __LOG2_H */
