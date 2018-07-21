#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <climits>
#include "audioreader.h"

static void readAudioPCM(
    size_t nchannel,
    float *input_buffer,
    float *output_buffer,
    size_t nsample
)
{
    size_t iChannel, iPoint, index = 0;
    for (iPoint = 0; iPoint < nsample; iPoint += nchannel)
    {
        output_buffer[index] = 0.0f;
        for (iChannel = 0; iChannel < nchannel ; iChannel++)
        {
            output_buffer[index] += fabs((input_buffer)[iPoint + iChannel]);
        }
        output_buffer[index++] /= nchannel;
    }
}

float *AudioReader::readAudio(
    size_t *nchannel,             // 声道个数
    size_t *sample_rate,          // 采样时间分辨率
    float *buffer,        // 采样流数据
    size_t *nsample,              // 采样点数
    size_t *output_buffer_length) // 输出流数据长度
{

    float *input_buffer = new float[(*nsample)];;
    readAudioPCM(*nchannel, buffer, input_buffer, *nsample);
    return input_buffer;
}

AudioReader::AudioReader()
{
}

AudioReader::~AudioReader()
{
}
