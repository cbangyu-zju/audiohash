#ifndef AUDIOCOMPARE_COMPARE_H
#define AUDIOCOMPARE_COMPARE_H

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
    Compare();
    ~Compare();
    double compare(double *template_hash, size_t template_nframes, int template_nfeatures, double *compare_hash, size_t compare_nframes, int compare_nfeatures);
    void get_mean_and_std(double *hash, size_t nframes, int nfeature, double *means, double *stds);
    double *preprocess(double *hash, size_t nframes, int nfeature, double *mean, double *std);
    double array_compare(double *hashA, double *hashB, size_t size);
};

#endif
