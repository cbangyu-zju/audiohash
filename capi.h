#ifndef AUDIOHASH_CAPI_H
#define AUDIOHASH_CAPI_H

#ifdef __cplusplus
extern "C" {
#endif

void *new_audiohash_config();

void delete_audiohash_config(void *config);

int set_pattern_audio(void *config, const char *filename);

float audio_compare(void *config, const char *filename);

#ifdef __cplusplus
}
#endif

#endif
