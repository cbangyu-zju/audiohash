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
    int MAX_FREQ;
    int MIN_FREQ;
    int STEP_FREQ;
    int NUM_HASH;
    
    CSpectral *mSpectral;

    HashCalculator(int frame_length, int step_length, int max_freq = 510, int min_freq = 10, int step_freq = 100);

    ~HashCalculator();

    double **calcHash(float *buf, size_t nSample, size_t *nFrames, int *nFeature, int sampleRate);

    double *calcHashEachWindow(double *buf, int sample_rate);
};

#endif
