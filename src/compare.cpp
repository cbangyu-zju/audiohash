#include <cstdio>


#include "compare.h"
#include <math.h>

using std::vector;

double Compare::array_compare(const double *hashA, const double *hashB, size_t size){
    float dot = 0.0, denom_a = 0.0, denom_b = 0.0 ;
    for(unsigned int i = 0u; i < size; ++i) {
        dot += hashA[i] * hashB[i] ;
        denom_a += hashA[i] * hashA[i] ;
        denom_b += hashB[i] * hashB[i] ;
    }
    return dot / (sqrt(denom_a) * sqrt(denom_b)) ;
}

void Compare::setPattern(double *hash, size_t nframes)
{
    pattern.hash = hash;
    pattern.nframes = nframes;
}

double Compare::compare(const double *hash, size_t nframes)
{
    size_t compare_nframes = pattern.nframes>=nframes?nframes:pattern.nframes;
    float score = array_compare(hash, pattern.hash, compare_nframes);
    return score;
}
