#ifndef AUDIOHASH_FFT_H
#define AUDIOHASH_FFT_H

extern "C" {

#include <complex.h>

}

class FFT
{
    double complex *Xt;
    double complex *twiddle_factors;

public:
    const int FFT_WINDOW;
    const int HALF_FFT;

    FFT(int window_size);
    ~FFT();

    void fft(const double *x, double complex *X);
};

#endif
