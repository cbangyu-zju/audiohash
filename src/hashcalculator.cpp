#include <cstdlib>
#include <cmath>
#include <cstring>
#include <stdint.h>
#include <stdbool.h>
#include "hashcalculator.h"


HashCalculator::HashCalculator(int frame_length, int step_length, int sample_rate, int max_freq, int min_freq, int step_freq){
    FRAME_LENGTH = frame_length;
    STEP_LENGTH = step_length;
    SAMPLE_RATE = sample_rate;
    MAX_FREQ = max_freq;
    MIN_FREQ = min_freq;
    STEP_FREQ = step_freq;
    mSpectral = new CSpectral(FRAME_LENGTH);
    NUM_HASH = (MAX_FREQ - MIN_FREQ) / step_freq;
}

HashCalculator::~HashCalculator()
{
    delete mSpectral;
}

void HashCalculator::calcHashEachWindow(double *data, double *hash){
    mSpectral->Pyulear(data, SAMPLE_RATE);
    double psd_tem = 0;
    for(int fre_i = 0;fre_i<NUM_HASH;fre_i++)
    {
        psd_tem = mSpectral->selectPSDFeature(MIN_FREQ+fre_i*STEP_FREQ,MIN_FREQ+(fre_i+1)*STEP_FREQ);
        hash[fre_i] =psd_tem;
    }
}

double *HashCalculator::calcHash(float *buf, size_t nSample, size_t *nFrames, int *nFeature)
{
    *nFeature = NUM_HASH;
    *nFrames = nSample / STEP_LENGTH - FRAME_LENGTH / STEP_LENGTH + 1;
    int length = (int)(*nFeature) * (int)(*nFrames);
    double *hash;
    hash = new double[length];
    double *temp_hash;
    temp_hash = new double[NUM_HASH];

    for(size_t iFrames = 0; iFrames < *nFrames; iFrames ++){
        double *buffer = new double[FRAME_LENGTH];
        for(int i = 0; i < FRAME_LENGTH; i++){
            if(buf[iFrames*STEP_LENGTH+i] == 0.0){
                buffer[i] = 0.000001;
            }else{
                buffer[i] = (double)buf[iFrames*STEP_LENGTH+i];
            }
        }
        calcHashEachWindow(buffer, temp_hash);
        for(int i = 0; i < NUM_HASH; i++){
            if(isnan(temp_hash[i]) == 0){
                hash[iFrames*NUM_HASH + i] = temp_hash[i];
            }else{
                hash[iFrames*NUM_HASH + i] = 0;
            }
        }
    }
    return hash;
}
