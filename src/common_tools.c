
#include "common_tools.h"

#if defined(WIN32) || defined(WIN64)
#define _WIN32_WINNT 0x0400
#include <windows.h>
#pragma comment(lib, "kernel32.lib")
#else /* !defined(WIN32) && !defined(WIN64) */
#include <unistd.h>
#endif /* defined(WIN32) || defined(WIN64) */

unsigned int GetNumberOfCores(void)
{
    unsigned int numberOfCores = 1;

#if defined(WIN32) || defined(WIN64)

    SYSTEM_INFO sysInfo;

    /* obtain system information */
    GetSystemInfo(&sysInfo);

    numberOfCores = sysInfo.dwNumberOfProcessors;

#else /* !defined(WIN32) && !defined(WIN64) */

#ifdef _SC_NPROCESSORS_CONF
    numberOfCores = sysconf(_SC_NPROCESSORS_CONF);
#endif /* _SC_NPROCESSORS_CONF */
    if (1 > numberOfCores)
    {
        numberOfCores = 1;
    }

#endif /* defined(WIN32) || defined(WIN64) */

    return numberOfCores;

} /* unsigned int GetNumberOfCores(void) */

static
int PRNG(int seed, int a, int b, int m)
{
    return (a * seed + b) % m;

} /* int PRNG(int seed, int a, int b, int m) */

eErrCode InitMatrix(int *pDst, size_t dstSize,
                    int seed, int a, int b, int m)
{
    size_t i;
    int sum = 0;
    int mean, remainder;

    /* check error(s) */
    if (NULL == pDst)
    {
        return ERR_NULL_POINTER;
    }

    /* initialize the destination array */
    for (i = 0; i < dstSize; i += 1)
    {
        seed = PRNG(seed, a, b, m);
        pDst[i] = seed;

        /* make a sum of all elements */
        sum += seed;
    }

    /* calculate the mean value. Avoid float logic when making rounding. */
    mean = sum / (signed) dstSize;
    remainder = sum - mean * (signed) dstSize;
    mean += (remainder * 2 > (signed) dstSize) ? (1) : (0);
    mean -= (remainder * 2 < -(signed) dstSize) ? (1) : (0);

    /* correct the array to make sum of all elements near to zero */
    for (i = 0; i < dstSize; i += 1)
    {
        pDst[i] -= mean;
    }

    return OK;

} /* eErrCode InitMatrix(int *pDst, size_t dstSize, */
