#ifndef AUDIOHASH_AUDIOREADER_H
#define AUDIOHASH_AUDIOREADER_H


class AudioReader
{
public:
    const unsigned SAMPLE_RATE;

    AudioReader(unsigned sr);
    ~AudioReader();

    float *readAudio(int32_t nchannel,
                     int32_t sample_rate,
                     float *buffer,
                     int32_t nsample,
                     int32_t *output_buffer_length);
};

#endif
