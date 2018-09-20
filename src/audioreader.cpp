#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <climits>


extern "C"{
#include <samplerate.h>
#include <mpg123.h>
}

#include <memory>
#include "audioreader.h"

using std::auto_ptr;

float *AudioReader::readAndDownSampleMp3(const char *filename,size_t *nframe)
{
    size_t origin_nframe;
    size_t origin_sample_rate;
    float *origin_buffer;
    origin_buffer = readMp3(filename, &origin_sample_rate, &origin_nframe);
    return downSamepleAudio(origin_buffer, &origin_sample_rate, &origin_nframe, nframe);
}

float *AudioReader::readMp3(
                            const char *filename,
                            size_t *sample_rate,
                            size_t *buflen)
{
    mpg123_handle *m_handle;
    int ret;
    
    if (mpg123_init() != MPG123_OK || (m_handle = mpg123_new(NULL, &ret)) == NULL || mpg123_open(m_handle, filename) != MPG123_OK)
    {
        printf("MPG123 error");
    }
    
    /*turn off logging */
    mpg123_param(m_handle, MPG123_ADD_FLAGS, MPG123_QUIET, 0);
    
    size_t totalsamples;
    
    mpg123_scan(m_handle);
    totalsamples = mpg123_length(m_handle);
    
    int iChannel, nChannel, encoding;
    if (mpg123_getformat(m_handle, (long*)sample_rate, &nChannel, &encoding) != MPG123_OK)
    {
        printf("Bad format");
    }
    
    mpg123_format_none(m_handle);
    mpg123_format(m_handle, *sample_rate, nChannel, encoding);
    
    size_t decbuflen = mpg123_outblock(m_handle);
    
    unsigned char *decbuf = new unsigned char[decbuflen];
    auto_ptr<unsigned char> decbuf_auto(decbuf);
    
    size_t nbsamples = totalsamples;
    nbsamples = (nbsamples < totalsamples) ? nbsamples : totalsamples;
    
    size_t iPoint, nPoint, index = 0, done;
    
    float *buffer = new float[nbsamples];
    auto_ptr<float> buffer_auto(buffer);
    
    *buflen = nbsamples;
    
    do
    {
        ret = mpg123_read(m_handle, decbuf, decbuflen, &done);
        //for(int read_i = 0; read_i < done; read_i++){printf("%d\n", abs(((char *)decbuf)[read_i]));}
        switch (encoding)
        {
            case MPG123_ENC_SIGNED_16 :
                nPoint = done / sizeof(short);
                for (iPoint = 0; iPoint < nPoint; iPoint += nChannel)
                {
                    buffer[index] = 0.0f;
                    for (iChannel = 0; iChannel < nChannel ; iChannel++)
                    {
                        buffer[index] += (((short *)decbuf)[iPoint + iChannel]) / (float)32767;
                    }
                    buffer[index++] /= nChannel;
                    if (index >= nbsamples) break;
                }
                break;
            case MPG123_ENC_SIGNED_8:
                nPoint = done / sizeof(char);
                for (iPoint = 0; iPoint < nPoint; iPoint += nChannel)
                {
                    buffer[index] = 0.0f;
                    for (iChannel = 0; iChannel < nChannel ; iChannel++)
                    {
                        buffer[index] += (((char *)decbuf)[iPoint + iChannel]) / (float)127;
                    }
                    buffer[index++] /= nChannel;
                    if (index >= nbsamples) break;
                }
                break;
            case MPG123_ENC_FLOAT_32:
                nPoint = done / sizeof(float);
                for (iPoint = 0; iPoint < nPoint; iPoint += nChannel)
                {
                    buffer[index] = 0.0f;
                    for (iChannel = 0; iChannel < nChannel; iChannel++)
                    {
                        buffer[index] += fabsf(((float *)decbuf)[iPoint + iChannel]);
                    }
                    buffer[index++] /= nChannel;
                    if (index >= nbsamples) break;
                }
                break;
            default:
                done = 0;
        }
    }
    while (ret == MPG123_OK && index < nbsamples);
    
    mpg123_close(m_handle);
    mpg123_delete(m_handle);
    mpg123_exit();
    
    return buffer_auto.release();
}

float *AudioReader::downSamepleAudio(
                                     float *buffer,
                                     size_t *input_sample_rate,
                                     size_t *input_nframe,
                                     size_t *output_nframe)
{
    long dist_sample_rate = SAMPLE_RATE;

    if (dist_sample_rate == *input_sample_rate)
    {
        *output_nframe = *input_nframe;
        return buffer;
    }

    auto_ptr<float> inbuffer_auto(buffer);

    /* resample float array */
    /* set distribution sample rate ratio */
    double sr_ratio = (double)(dist_sample_rate) / (double)(*input_sample_rate);

    /* allocate output buffer for conversion */
    *output_nframe = (size_t)(sr_ratio * (*input_nframe));
    float *output_buffer = new float[int(*output_nframe)];
    auto_ptr<float> output_buffer_auto(output_buffer);

    int error;
    SRC_STATE *src_state = src_new(SRC_LINEAR, 1, &error);

    SRC_DATA src_data;
    src_data.data_in = buffer;
    src_data.data_out = output_buffer;
    src_data.input_frames = *input_nframe;
    src_data.output_frames = *output_nframe;
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
