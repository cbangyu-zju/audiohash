#include <math.h>
#include "compareaudio.h"


using std::vector;

CompareAudio::CompareAudio(int step_in_second)
 :STEP_IN_SECOND(step_in_second),SAMPLERATE(1000)
{
    reader = new AudioReader(SAMPLERATE);
    calculator = new  HashCalculator(600, 200, SAMPLERATE);
    compare = new Compare();
}

CompareAudio::~CompareAudio()
{
    delete reader;
    delete calculator;
    delete compare;}

int CompareAudio::read_mp3file(const char *mp3file, float *buffer, size_t *nframe){
    int error = 0;
    try
    {
        buffer = reader->readAndDownSampleMp3(mp3file, nframe);
    }
    catch (std::exception &e)
    {
        fprintf(stderr, "%s\n", e.what());
        error = -1;
    }
    catch (...)
    {
        fprintf(stderr, "Unexpected error\n");
        error = -1;
    }
    return error;
}


float CompareAudio::compare_two_mp3file(const char *template_mp3file, const char *compare_mp3file)
{
    float result = -1.0;
    double score = 0.0;
    try{
        float *template_buffer = reader->readAndDownSampleMp3(template_mp3file, &template_nframe);;
        float *compare_buffer = reader->readAndDownSampleMp3(compare_mp3file, &compare_nframe);;
        if(STEP_IN_SECOND >= template_nframe){
            fprintf(stderr, "Template Mp3 File Too Short Duration");
            return result;
        }
        if(STEP_IN_SECOND >= compare_nframe){
            fprintf(stderr, "Compare Mp3 File Too Short Duration");
            return result;
        }
        size_t nframe = template_nframe>=compare_nframe?compare_nframe:template_nframe;
        size_t start_frame = 0;
        int step_count = 0;
        float *template_buffer_temp = new float[STEP_IN_SECOND];
        float *compare_buffer_temp = new float[STEP_IN_SECOND];
        while (start_frame <= (nframe-STEP_IN_SECOND)) {
            for(int iframe = 0; iframe < STEP_IN_SECOND; iframe++){
                template_buffer_temp[iframe] = template_buffer[iframe+start_frame];
                compare_buffer_temp[iframe] = compare_buffer[iframe+start_frame];
            }
            size_t template_nwindow, compare_nwindow;
            int template_nfeature, compare_nfeature;
            double *template_hash = calculator->calcHash(template_buffer_temp, size_t(STEP_IN_SECOND), &template_nwindow, &template_nfeature);
            double *compare_hash = calculator->calcHash(compare_buffer_temp, size_t(STEP_IN_SECOND), &compare_nwindow, &compare_nfeature);
            score += compare->compare(template_hash, template_nwindow, template_nfeature, compare_hash, compare_nwindow, compare_nfeature);
            start_frame += STEP_IN_SECOND;
            step_count += 1;
        }
        return (float)(score/step_count);
    }catch (std::exception &e)
    {
        fprintf(stderr, "%s\n", e.what());
        result = -1;
    }
    catch (...)
    {
        fprintf(stderr, "Unexpected error\n");
        result = -1;
    }
    return result;
}
