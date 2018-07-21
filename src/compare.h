#ifndef AUDIOHASH_COMPARE_H
#define AUDIOHASH_COMPARE_H

#include <vector>

struct HashPattern
{
    double **hash;
    double *mean;
    double *std;
    size_t nframes;
    int nfeature;
};

class Compare{

public:
    HashPattern pattern;
    void setPattern(double **hash, size_t nframes, int nfeature);
    double compare(double **hash, size_t nframes);
    double *preprocess(double **hash, size_t nframes, int nfeature, double *mean, double *std);
    double array_compare(double *hashA, double *hashB, size_t size);
};

#endif
