#ifndef __RDTSC_H_DEFINED__
#define __RDTSC_H_DEFINED__

#include <stdint.h>

#if defined(__i386__)

static __inline__ uint64_t rdtsc(void)
{
    uint64_t int x;
    __asm__ volatile(".byte 0x0f, 0x31"
                     : "=A"(x));
    return x;
}
#elif defined(__x86_64__)

static __inline__ uint64_t rdtsc(void)
{
    unsigned hi, lo;
    __asm__ __volatile__("rdtsc"
                         : "=a"(lo), "=d"(hi));
    return ((uint64_t)lo) | (((uint64_t)hi) << 32);
}

#elif defined(__powerpc__)

static __inline__ uint64_t rdtsc(void)
{
    uint64_t int result = 0;
    unsigned long int upper, lower, tmp;
    __asm__ volatile(
        "0:                  \n"
        "\tmftbu   %0           \n"
        "\tmftb    %1           \n"
        "\tmftbu   %2           \n"
        "\tcmpw    %2,%0        \n"
        "\tbne     0b         \n"
        : "=r"(upper), "=r"(lower), "=r"(tmp));
    result = upper;
    result = result << 32;
    result = result | lower;

    return (result);
}

#endif

/*  $RCSfile:  $   $Author: kazutomo $
 *  $Revision: 1.6 $  $Date: 2005/04/13 18:49:58 $
 */

#endif
