#ifndef AUDIOHASH_AUDIOREADER_H
#define AUDIOHASH_AUDIOREADER_H


class AudioReader
{
public:
    const unsigned SAMPLE_RATE;

    AudioReader(unsigned sr);
    ~AudioReader();

    float *readaudio(const char *filename, size_t *buflen);
};

#endif
