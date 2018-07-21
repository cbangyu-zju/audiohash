#ifndef AUDIOHASH_AUDIOHASH_H
#define AUDIOHASH_AUDIOHASH_H

#include "Spectral.h"

class HashCalculator
{

public:
    // the count of samples to generate a single hash value
    int FRAME_LENGTH;

    // the step length each time move fft window
    int STEP_LENGTH;
    
    int SAMPLE_RATE;
    
    int MAX_FREQ;
    int MIN_FREQ;
    int STEP_FREQ;
    int NUM_HASH;
    
    CSpectral *mSpectral;

    HashCalculator(int frame_length, int step_length, int sample_rate, int max_freq = 510, int min_freq = 10, int step_freq = 100);

    ~HashCalculator();

    double **calcHash(float *buf, size_t nSample, size_t *nFrames, int *nFeature);

    double *calcHashEachWindow(double *buf);
};

#endif
