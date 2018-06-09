#ifndef AUDIOHASH_CAPI_H
#define AUDIOHASH_CAPI_H

#ifdef __cplusplus
extern "C" {
#endif

void *new_audiohash_config();

void delete_audiohash_config(void *config);

int set_pattern_audio(void *config,
                      const char *stream_type,
                      size_t *nchannel,
                      size_t *sample_resolution,
                      size_t *sample_rate,
                      unsigned char *buffer,
                      size_t *nsample);

float audio_compare(void *config,
                    const char *stream_type,
                    size_t *nchannel,
                    size_t *sample_resolution,
                    size_t *sample_rate,
                    unsigned char *buffer,
                    size_t *nsample);

#ifdef __cplusplus
}
#endif

#endif
