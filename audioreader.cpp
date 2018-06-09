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

static
float *readaudio_mp3(
    const char *filename,
    long *sr,
    const float nbsecs,
    size_t *buflen)
{
    mpg123_handle *m;
    int ret;

    if (mpg123_init() != MPG123_OK ||
            (m = mpg123_new(NULL, &ret)) == NULL ||
            mpg123_open(m, filename) != MPG123_OK)
    {
        throw FileOpenError();
    }

    /*turn off logging */
    mpg123_param(m, MPG123_ADD_FLAGS, MPG123_QUIET, 0);

    size_t totalsamples;

    mpg123_scan(m);
    totalsamples = mpg123_length(m);

    int iChannel, nChannel, encoding;
    if (mpg123_getformat(m, sr, &nChannel, &encoding) != MPG123_OK)
    {
        throw BadFormatError();
    }

    mpg123_format_none(m);
    mpg123_format(m, *sr, nChannel, encoding);

    size_t decbuflen = mpg123_outblock(m);

    unsigned char *decbuf = new unsigned char[decbuflen];
    auto_ptr<unsigned char> decbuf_auto(decbuf);

    size_t nbsamples = (nbsecs <= 0) ? totalsamples : nbsecs * (*sr);
    nbsamples = (nbsamples < totalsamples) ? nbsamples : totalsamples;

    size_t iPoint, nPoint, index = 0, done;

    float *buffer = new float[nbsamples];
    auto_ptr<float> buffer_auto(buffer);

    *buflen = nbsamples;

    do
    {
        ret = mpg123_read(m, decbuf, decbuflen, &done);
        switch (encoding)
        {
        case MPG123_ENC_SIGNED_16 :
            nPoint = done / sizeof(short);
            for (iPoint = 0; iPoint < nPoint; iPoint += nChannel)
            {
                buffer[index] = 0.0f;
                for (iChannel = 0; iChannel < nChannel ; iChannel++)
                {
                    buffer[index] += abs(((short *)decbuf)[iPoint + iChannel]) / (float)SHRT_MAX;
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
                    buffer[index] += abs(((char *)decbuf)[iPoint + iChannel]) / (float)SCHAR_MAX;
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

    mpg123_close(m);
    mpg123_delete(m);
    mpg123_exit();

    return buffer_auto.release();
}

static
float *readaudio_snd(
    const char *filename,
    long *sr,
    const float nbsecs,
    size_t *buflen)
{

    SF_INFO sf_info;
    sf_info.format = 0;
    SNDFILE *sndfile = sf_open(filename, SFM_READ, &sf_info);
    if (sndfile == NULL)
    {
        throw FileOpenError();
    }

    /* normalize */
    sf_command(sndfile, SFC_SET_NORM_FLOAT, NULL, SF_TRUE);

    *sr = (long)sf_info.samplerate;

    //allocate input buffer for signal
    sf_count_t src_frames = (nbsecs <= 0) ? sf_info.frames : (nbsecs * sf_info.samplerate);
    src_frames = (sf_info.frames < src_frames) ? sf_info.frames : src_frames;
    float *inbuf = new float[src_frames * sf_info.channels];
    auto_ptr<float> inbuf_auto(inbuf);

    /*read frames */
    sf_count_t cnt_frames = sf_readf_float(sndfile, inbuf, src_frames);

    float *buf = new float[cnt_frames];
    auto_ptr<float> buf_auto(buf);

    //average across all channels
    sf_count_t  i, j, indx = 0;
    for (i = 0; i < cnt_frames * sf_info.channels; i += sf_info.channels)
    {
        buf[indx] = 0;
        for (j = 0; j < sf_info.channels; j++)
        {
            buf[indx] += fabsf(inbuf[i + j]);
        }
        buf[indx++] /= sf_info.channels;
    }

    *buflen = indx;
    sf_close(sndfile);
    return buf_auto.release();
}

float *AudioReader::readaudio(
    const char *filename,
    size_t *buflen)
{
    assert (NULL != filename);

    long orig_sr;
    long sr = SAMPLE_RATE;
    size_t inbufferlength;
    *buflen = 0;

    const char *suffix = strrchr(filename, '.');
    if (NULL == suffix)
    {
        throw InvalidInputError();
    }

    float *inbuffer;
    if (!strcasecmp(suffix, ".mp3"))
    {
        inbuffer = readaudio_mp3(filename, &orig_sr, 0, &inbufferlength);
    }
    else
    {
        inbuffer = readaudio_snd(filename, &orig_sr, 0, &inbufferlength);
    }

    if (sr == orig_sr)
    {
        *buflen = inbufferlength;
        return inbuffer;
    }

    auto_ptr<float> inbuffer_auto(inbuffer);

    /* resample float array */
    /* set desired sr ratio */
    double sr_ratio = (double)(sr) / (double)orig_sr;
    if (src_is_valid_ratio(sr_ratio) == 0)
    {
        throw ResampleError();
    }

    /* allocate output buffer for conversion */
    size_t outbufferlength = sr_ratio * inbufferlength;
    float *outbuffer = new float[outbufferlength];
    auto_ptr<float> outbuffer_auto(outbuffer);

    int error;
    SRC_STATE *src_state = src_new(SRC_LINEAR, 1, &error);
    if (!src_state)
    {
        throw ResampleError();
    }

    SRC_DATA src_data;
    src_data.data_in = inbuffer;
    src_data.data_out = outbuffer;
    src_data.input_frames = inbufferlength;
    src_data.output_frames = outbufferlength;
    src_data.end_of_input = SF_TRUE;
    src_data.src_ratio = sr_ratio;

    /* sample rate conversion */
    error = src_process(src_state, &src_data);
    if (0 != error)
    {
        src_delete(src_state);
        throw ResampleError();
    }

    *buflen = src_data.output_frames;

    src_delete(src_state);

    return outbuffer_auto.release();
}

AudioReader::AudioReader(unsigned sr)
    : SAMPLE_RATE(sr)
{
}

AudioReader::~AudioReader()
{
}