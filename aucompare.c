#include <stdio.h>
#include <string.h>
#include <math.h>
#include "capi.h"

void printHelp()
{
    fprintf(stderr, "compare <inputaudiofile> <patternaudiofile> \n");
}

int main(int argc, char **argv)
{
    void *config = NULL;
    int error = 0;
    double score = 0;

    if (3 > argc)
    {
        printHelp();
        return -1;
    }

    config = new_audiohash_config();

    error = set_pattern_audio(config, argv[2]);
    if (0 != error)
    {
        fprintf(stderr, "Error occurs while setting %s\n", argv[2]);
        delete_audiohash_config(config);
        return -1;
    }

    score = audio_compare(config, argv[1]);

    if (0 == score)
    {
        fprintf(stderr, "Error occurs while compare pattern in %s\n", argv[1]);
    }
    else
    {
        printf("%s and %s socre: %8.4lf\n", argv[1], argv[2], score);
    }

    return 0;
}
