#ifndef AUDIOCOMPARE_COMPAREAUDIO_H
#define AUDIOCOMPARE_COMPAREAUDIO_H

#include <vector>
#include <cstdio>
#include <stdint.h>
#include "capi.h"
#include "compare.h"
#include "hashcalculator.h"
#include "audioreader.h"


class CompareAudio{
    
public:
    CompareAudio(int step_in_second);
    ~CompareAudio();
    
    int SAMPLERATE;
    AudioReader * reader;
    HashCalculator * calculator;
    Compare *compare;
    size_t template_nframe, compare_nframe;
    const int STEP_IN_SECOND;

    
    int read_mp3file(const char *mp3file, float *buffer, size_t *nframe);
    float compare_two_mp3file(const char *template_mp3file, const char *compare_mp3file);
};

#endif
