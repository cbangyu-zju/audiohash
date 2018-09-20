#include <stdio.h>
#include <string.h>
#include "capi.h"


void printHelp()
{
    fprintf(stderr, "compare audio \n");
}

int main(int argc, char **argv)
{
    
    const char *template_mp3 = "../music/standard.mp3";
    const char *compare_mp3 = "../music/compare_good.mp3";
    float score = compare_two_mp3file(template_mp3, compare_mp3);
    printf("score: %f\n", score);
    return 0;
}
