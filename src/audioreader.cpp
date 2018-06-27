#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <climits>


extern "C"{

#include "samplerate.h"

}
#include <memory>
#include "audioreader.h"

using std::auto_ptr;

static void readAudioPCM(
                         size_t nchannel,
                         size_t sample_resolution,
                         unsigned char *input_buffer,
                         float *output_buffer,
                         size_t nsample
                         )
{
    size_t iChannel, iPoint, index = 0;
    
    switch (sample_resolution)
    {
        case 8:
            for (iPoint = 0; iPoint < nsample; iPoint += nchannel)
            {
                output_buffer[index] = 0.0f;
                for (iChannel = 0; iChannel < nchannel ; iChannel++)
                {
                    output_buffer[index] += abs(((char *)input_buffer)[iPoint + iChannel]) / (float)SCHAR_MAX;
                }
                output_buffer[index++] /= nchannel;
            }
            break;
        case 16 :
            for (iPoint = 0; iPoint < nsample; iPoint += nchannel)
            {
                output_buffer[index] = 0.0f;
                for (iChannel = 0; iChannel < nchannel ; iChannel++)
                {
                    output_buffer[index] += abs(((short *)input_buffer)[iPoint + iChannel]) / (float)SHRT_MAX;
                }
                output_buffer[index++] /= nchannel;
            }
            break;
        case 32:
            for (iPoint = 0; iPoint < nsample; iPoint += nchannel)
            {
                output_buffer[index] = 0.0f;
                for (iChannel = 0; iChannel < nchannel; iChannel++)
                {
                    output_buffer[index] += fabsf(((float *)input_buffer)[iPoint + iChannel]);
                }
                output_buffer[index++] /= nchannel;
            }
            break;
        default:
            break;
    }
}
float *AudioReader::readAudio(
    int32_t nchannel,             // 声道个数
    int32_t sample_rate,          // 采样时间分辨率
    float *buffer,        // 采样流数据
    int32_t nsample,              // 采样点数
    int32_t *output_buffer_length) // 输出流数据长度
{
    long dist_sample_rate = SAMPLE_RATE;

    if (dist_sample_rate == sample_rate)
    {
        *output_buffer_length = nsample;
        printf("hello1\n");
        return buffer;
    }

    auto_ptr<float> inbuffer_auto(buffer);

    /* resample float array */
    /* set distribution sample rate ratio */
    double sr_ratio = (double)(dist_sample_rate) / (double)(sample_rate);

    /* allocate output buffer for conversion */
    *output_buffer_length = (int32_t)(sr_ratio * (nsample));
    float *output_buffer = new float[int(*output_buffer_length)];
    auto_ptr<float> output_buffer_auto(output_buffer);

    int error;
    SRC_STATE *src_state = src_new(SRC_LINEAR, 1, &error);

    SRC_DATA src_data;
    src_data.data_in = buffer;
    src_data.data_out = output_buffer;
    src_data.input_frames = nsample;
    src_data.output_frames = *output_buffer_length;
    src_data.end_of_input = 1;
    src_data.src_ratio = sr_ratio;

    /* sample rate conversion */
    src_process(src_state, &src_data);
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
