#include <cstdio>

extern "C" {
#include <stdint.h>
}

#include "compare.h"

using std::vector;

const uint32_t MASK_01010101 = (((uint32_t)(-1)) / 3);
const uint32_t MASK_00110011 = (((uint32_t)(-1)) / 5);
const uint32_t MASK_00001111 = (((uint32_t)(-1)) / 17);

inline int bitcount(uint32_t n)
{
    n = (n & MASK_01010101) + ((n >> 1) & MASK_01010101) ;
    n = (n & MASK_00110011) + ((n >> 2) & MASK_00110011) ;
    n = (n & MASK_00001111) + ((n >> 4) & MASK_00001111) ;
    return n % 255;
}

// a predefined shortcut
inline float BERThreshold(size_t size)
{
    float thresholds[] = {0.05, 0.09, 0.20, 0.24, 0.27, 0.29};
    unsigned nl = 6, il = size/10;

    return il >= nl? thresholds[nl-1] : thresholds[il]; 
}

unsigned block_bitcount(const uint32_t *hashA, const uint32_t *hashB, const unsigned block_size)
{
    unsigned result = 0;
    for (unsigned i = 0; i < block_size; i++)
    {
        uint32_t xordhash = hashA[i] ^ hashB[i];
        result += bitcount(xordhash);
    }

    return result;
}

void Compare::setPattern(uint32_t *hash, uint32_t nframes, const char *name)
{
    pattern = {hash,nframes};
}

float Compare::compare(const uint32_t *hashA, size_t nframes)
{
    return 0.5;
}
