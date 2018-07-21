#include <math.h>
#include "compare.h"

using std::vector;

Compare::Compare(){
    pattern = new HashPattern();
}

Compare::~Compare()
{
    delete pattern;
}

double *Compare::preprocess(double **hash, size_t nframes, int nfeature, double *mean, double *std){
    double *feature = new double[nframes*nfeature];
    for(int j = 0; j < nfeature; j++){
        for(int i = 0; i < nframes; i++){
            feature[i*nfeature + j] = (hash[i][j] - mean[j]) / std[j];
        }
    }
    return feature;
}

double Compare::array_compare(double *hashA, double *hashB, size_t size){
    float dot = 0.0, denom_a = 0.0, denom_b = 0.0 ;
    for(unsigned int i = 0u; i < size; ++i) {
        dot += hashA[i] * hashB[i] ;
        denom_a += hashA[i] * hashA[i] ;
        denom_b += hashB[i] * hashB[i] ;
    }
    float result = (sqrt(denom_a) * sqrt(denom_b));
    if(result == 0.0){
        return 0;
    }else{
        return dot / result;
    }
}

void Compare::setPattern(double **hash, size_t nframes, int nfeature)
{
    pattern->hash = hash;
    pattern->nframes = nframes;
    pattern->nfeature = nfeature;
    pattern->mean = new double[nfeature];
    pattern->std = new double[nfeature];
    for(int i = 0; i< nfeature; i++){
        double mean = 0.0;
        double std = 0.0;
        for(int j = 0; j<nframes;j++){
            mean += hash[j][i];
        }
        mean = mean / nframes;
        for(int j = 0; j<nframes;j++){
            std += (hash[j][i] - mean)*(hash[j][i] - mean);
        }
        std = sqrt(std) / nframes;
        if(std == 0){std = 1;}
        pattern->mean[i] = mean;
        pattern->std[i] = std;
    }

}
 

double Compare::compare(double **hash, size_t nframes)
{
    size_t compare_nframes = pattern->nframes>=nframes?nframes:pattern->nframes;
    double *feature_pattern = preprocess(pattern->hash, compare_nframes, pattern->nfeature, pattern->mean, pattern->std);
    double *feature_compare = preprocess(hash, compare_nframes, pattern->nfeature, pattern->mean, pattern->std);
    float score = array_compare(feature_pattern, feature_compare, compare_nframes);
    return score;
}
