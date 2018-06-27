#ifndef AUDIOHASH_CAPI_H
#define AUDIOHASH_CAPI_H

#include <stdint.h>

void *new_audiohash_config();

void delete_audiohash_config(void *config);

int set_pattern_audio(void *config,
                      int32_t nchannel,
                      int32_t sample_rate,
                      float *buffer,
                      int32_t nsample);

float audio_compare(void *config,
                    int32_t nchannel,
                    int32_t sample_rate,
                    float *buffer,
                    int32_t nsample);

#endif
