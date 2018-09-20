#include "compareaudio.h"

float compare_two_mp3file(const char *template_mp3file, const char *compare_mp3file){
    int step_in_second = 2000; // 2000ms
    CompareAudio *compare_audio = new CompareAudio(step_in_second);
    float score = compare_audio->compare_two_mp3file(template_mp3file, compare_mp3file);
    return score;
    
}
