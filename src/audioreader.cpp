#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <climits>

extern "C" {

#include <sndfile.h>
#include <samplerate.h>
#include <mpg123.h>

}

#include <memory>
#include "error.h"
#include "audioreader.h"

using std::auto_ptr;

static float * readAudioPCM(
    size_t nchannel,
    size_t sample_resolution,
    unsigned char *input_buffer,
    size_t nsample
)
{
    float *buffer = new float[int(nsample)];

    size_t iChannel, iPoint, index = 0;

    switch (sample_resolution)
        {
        case 8:
            for (iPoint = 0; iPoint < nsample; iPoint += nchannel)
            {
                input_buffer[index] = 0.0f;
                for (iChannel = 0; iChannel < nchannel ; iChannel++)
                {
                    input_buffer[index] += abs(((char *)input_buffer)[iPoint + iChannel]) / (float)SCHAR_MAX;
                }
                buffer[index++] /= nchannel;
            }
            break;
        case 16 :
            for (iPoint = 0; iPoint < nsample; iPoint += nchannel)
            {
                input_buffer[index] = 0.0f;
                for (iChannel = 0; iChannel < nchannel ; iChannel++)
                {
                    input_buffer[index] += abs(((short *)input_buffer)[iPoint + iChannel]) / (float)SHRT_MAX;
                }
                buffer[index++] /= nchannel;
            }
            break;
        case 32:
            for (iPoint = 0; iPoint < nsample; iPoint += nchannel)
            {
                input_buffer[index] = 0.0f;
                for (iChannel = 0; iChannel < nchannel; iChannel++)
                {
                    input_buffer[index] += fabsf(((float *)input_buffer)[iPoint + iChannel]);
                }
                buffer[index++] /= nchannel;
            }
            break;
        default:
            index = 0;
        }
    return buffer;
}

float *AudioReader::readAudio(
    const char *stream_type,
    size_t *nchannel,
    size_t *sample_resolution,
    size_t *sample_rate,
    unsigned char *buffer,
    size_t *nsample,
    size_t *output_buffer_length)
{
    long dist_sample_rate = SAMPLE_RATE;

    float *input_buffer;
    input_buffer = readAudioPCM(*nchannel, *sample_resolution, buffer, *nsample);

    if (dist_sample_rate == sample_rate)
    {
        *output_buffer_length = *nsample;
        return input_buffer;
    }

    auto_ptr<float> inbuffer_auto(input_buffer);

    /* resample float array */
    /* set distribution sample rate ratio */
    double sr_ratio = (double)(dist_sample_rate) / (double)(*sample_rate);
    if (src_is_valid_ratio(sr_ratio) == 0)
    {
        throw ResampleError();
    }

    /* allocate output buffer for conversion */
    output_buffer_length = (size_t)(sr_ratio * nsample);
    float *output_buffer = new float[int(output_buffer_length)];
    auto_ptr<float> output_buffer_auto(output_buffer);

    int error;
    SRC_STATE *src_state = src_new(SRC_LINEAR, 1, &error);
    if (!src_state)
    {
        throw ResampleError();
    }

    SRC_DATA src_data;
    src_data.data_in = input_buffer;
    src_data.data_out = output_buffer;
    src_data.input_frames = *nsample;
    src_data.output_frames = *output_buffer_length;
    src_data.end_of_input = SF_TRUE;
    src_data.src_ratio = sr_ratio;

    /* sample rate conversion */
    error = src_process(src_state, &src_data);
    if (0 != error)
    {
        src_delete(src_state);
        throw ResampleError();
    }
    src_delete(src_state);

    return output_buffer_auto.release();
}

AudioReader::AudioReader(unsigned sr)
    : SAMPLE_RATE(sr)
{
}

AudioReader::~AudioReader()
{
}