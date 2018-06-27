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
    int32_t nchannel = 1;
    int32_t sample_rate = 8000;
    int32_t nsample = 8000;
    float buffer[8000] = {16};
    fill_n(buffer, 8000, 16);
    config = new_audiohash_config();

    error = set_pattern_audio(config, nchannel, sample_rate, buffer, nsample);
    if (0 != error)
    {
        delete_audiohash_config(config);
        return -1;
    }

    score = audio_compare(config, nchannel, sample_rate, buffer, nsample);

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
    printf("score: %f\n", score);
    return 0;
}
