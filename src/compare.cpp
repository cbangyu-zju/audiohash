#include <math.h>
#include "compare.h"

using std::vector;

Compare::Compare(){
}

Compare::~Compare()
{
}

double *Compare::preprocess(double *hash, size_t nframes, int nfeature, double *mean, double *std){
    double *feature = new double[nframes*nfeature];
    for(int i = 0; i < nfeature; i++){
        for(int j = 0; j < nframes; j++){
            feature[i*nframes + i] = (hash[i*nframes + i] - mean[i]) / std[i];
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

void Compare::get_mean_and_std(double *hash, size_t nframes, int nfeature, double *means, double *stds)
{
    for(int i = 0; i< nfeature; i++){
        double mean = 0.0;
        double std = 0.0;
        for(int j = 0; j<nframes;j++){
            mean += hash[j*nfeature + i];
        }
        mean = mean / nframes;
        for(int j = 0; j<nframes;j++){
            std += (hash[j*nfeature + i] - mean)*(hash[j*nfeature + i] - mean);
        }
        std = sqrt(std) / nframes;
        if(std == 0){std = 1;}
        means[i] = mean;
        stds[i] = std;
    }

}
 

double Compare::compare(double *template_hash, size_t template_nframes, int template_nfeatures, double *compare_hash, size_t compare_nframes, int compare_nfeatures)
{
    size_t nframe = template_nframes>=compare_nframes?compare_nframes:template_nframes;
    double *means, *stds;
    means = new double[template_nfeatures];
    stds = new  double[template_nfeatures];
    get_mean_and_std(template_hash, nframe, template_nfeatures, means, stds);
    double *feature_template = preprocess(template_hash, nframe, template_nfeatures, means, stds);
    double *feature_compare = preprocess(compare_hash, compare_nframes, template_nfeatures, means, stds);
    float score = array_compare(feature_template, feature_compare, nframe);
    delete means;
    delete stds;
    return score;
}
