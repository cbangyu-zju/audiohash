#ifndef AUDIOHASH_COMPARE_H
#define AUDIOHASH_COMPARE_H

#include <vector>

struct HashPattern
{
    uint32_t *hash;
    uint32_t nframes;
};

class Compare{

public:
    HashPattern pattern;
    void setPattern(uint32_t *hash, uint32_t nframes, const char *name);
    float compare(const uint32_t *hash, size_t nframes);
};

#endif
