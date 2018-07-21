#ifndef AUDIOHASH_AUDIOREADER_H
#define AUDIOHASH_AUDIOREADER_H


class AudioReader
{
public:
    AudioReader();
    ~AudioReader();

    float *readAudio(size_t *nchannel,
                     size_t *sample_rate,
                     float *buffer,
                     size_t *nsample,
                     size_t *output_buffer_length);
};

#endif
