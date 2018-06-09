#ifndef AUDIOHASH_AUDIOREADER_H
#define AUDIOHASH_AUDIOREADER_H


class AudioReader
{
public:
    const unsigned SAMPLE_RATE;

    AudioReader(unsigned sr);
    ~AudioReader();

    float *readAudio(const char *stream_type,
                     size_t *nchannel,
                     size_t *sample_resolution,
                     size_t *sample_rate,
                     unsigned char *buffer,
                     size_t *nsample,
                     size_t *output_buffer_length);
};

#endif
