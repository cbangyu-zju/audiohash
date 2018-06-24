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
    const char *stream_type,      // 音频流类型，目前只支持 PCM，WAV 格式的数据流也是PCM
    size_t *nchannel,             // 声道个数
    size_t *sample_resolution,    // 采样位数，8位/16位/32位
    size_t *sample_rate,          // 采样时间分辨率
    unsigned char *buffer,        // 采样流数据
    size_t *nsample,              // 采样点数
    size_t *output_buffer_length) // 输出流数据长度
{
    long dist_sample_rate = SAMPLE_RATE;

    float *input_buffer = new float[(*nsample)];;
    readAudioPCM(*nchannel, *sample_resolution, buffer, input_buffer, *nsample);
    if (dist_sample_rate == *sample_rate)
    {
        *output_buffer_length = *nsample;
        return input_buffer;
    }

    auto_ptr<float> inbuffer_auto(input_buffer);

    /* resample float array */
    /* set distribution sample rate ratio */
    double sr_ratio = (double)(dist_sample_rate) / (double)(*sample_rate);

    /* allocate output buffer for conversion */
    *output_buffer_length = (size_t)(sr_ratio * (*nsample));
    float *output_buffer = new float[int(*output_buffer_length)];
    auto_ptr<float> output_buffer_auto(output_buffer);

    int error;
    SRC_STATE *src_state = src_new(SRC_LINEAR, 1, &error);

    SRC_DATA src_data;
    src_data.data_in = input_buffer;
    src_data.data_out = output_buffer;
    src_data.input_frames = *nsample;
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
