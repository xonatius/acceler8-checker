
#if !defined(__COMMON_TOOLS_H)
#define __COMMON_TOOLS_H

#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/* Declare return values */
typedef
enum eErrCode
{
    OK = 0,

    /* some unexpected unknown error */
    ERR_UNKNOWN = -1,
    /* one or more pointers passed to a function are NULL */
    ERR_NULL_POINTER = -2,
    /* one or more integral parameters passed to a function are out of bound */
    ERR_OUT_OF_BOUND = -3

} eErrCode;

/*
    Declare CPU management functions
*/

/* Obtain the number of available CPU cores */
unsigned int GetNumberOfCores(void);

/* Initialize the matrix with pseudo-random values. pDst is a pointer to
   the destination matrix of dstSize items. seed, a, b and modulo are parameters
   for pseudo-random generations. Sum of any sub-matrix of the destination
   matrix should not lead to int wraparound. */
eErrCode InitMatrix(int *pDst, size_t dstSize,
                    int seed, int a, int b, int m);

#ifdef __cplusplus
} // extern "C"
#endif /* __cplusplus */

#endif /* !defined(__COMMON_TOOLS_H) */
