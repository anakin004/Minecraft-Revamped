#pragma once

// i think this is windows specific,
// did some reasearch about how floor is very slow 
// saw this on stack overflow

/*
 fast float function - > https://stackoverflow.com/questions/2352303/avoiding-calls-to-floor
*/

/*
_mm_load_ss(&f) takes the input float f and places it in an SSE register.
_mm_cvtt_ss2si converts the value in the SSE register to an integer using truncation.
The resulting integer is returned.
*/
// it seems that simd instructions execute faster than general-purpose cpu instructions for certain ops
#include <emmintrin.h>

inline int ftoi_fast(float f)
{
    return _mm_cvtt_ss2si(_mm_load_ss(&f));     // SSE1 instructions for float->int
}