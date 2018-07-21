#ifndef AUDIOHASH_COMPARE_H
#define AUDIOHASH_COMPARE_H

#include <vector>

struct HashPattern
{
    double *hash;
    size_t nframes;
};

class Compare{

public:
    HashPattern pattern;
    void setPattern(double *hash, size_t nframes);
    double compare(const double *hash, size_t nframes);
    double array_compare(const double *hashA, const double *hashB, size_t size);
};

#endif
