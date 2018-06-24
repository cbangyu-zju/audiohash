#include <stdio.h>
#include <string.h>
#include <math.h>
#include "capi.h"
#include <algorithm>

using namespace std;

void printHelp()
{
    fprintf(stderr, "compare audio \n");
}

int main(int argc, char **argv)
{
    void *config = NULL;
    int error = 0;
    float score = 0;
    const char* stream_type = "PCM";
    size_t nchannel = 1;
    size_t sample_resolution = 8;
    size_t sample_rate = 8000;
    size_t nsample = 8000;
    unsigned char buffer[8000] = {16};
    fill_n(buffer, 8000, 16);
    config = new_audiohash_config();

    error = set_pattern_audio(config, stream_type, &nchannel, &sample_resolution, &sample_rate, buffer, &nsample);
    if (0 != error)
    {
        delete_audiohash_config(config);
        return -1;
    }

    score = audio_compare(config, stream_type, &nchannel, &sample_resolution, &sample_rate, buffer, &nsample);

    /*
    if (0 == score)
    {
        fprintf(stderr, "Error occurs while compare pattern");
    }
    else
    {
        printf("socre: %8.4lf\n", score);
    }
    */
    return 0;
}
