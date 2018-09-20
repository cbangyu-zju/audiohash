#ifndef AUDIOCOMPARE_AUDIOREADER_H
#define AUDIOCOMPARE_AUDIOREADER_H


class AudioReader
{
public:
    const unsigned SAMPLE_RATE;

    AudioReader(unsigned sr);
    ~AudioReader();
    
    float *readAndDownSampleMp3(const char *filename, size_t *nframe);
    float *readMp3(const char *filename, size_t * sample_rate, size_t *nframe);
    float *downSamepleAudio(
                     float *buffer,
                     size_t *input_sample_rate,
                     size_t *input_nframe,
                     size_t *output_nframe);
};

#endif
