#include <cmath>

extern "C" {
#include <stdbool.h>
#include <complex.h>
}

#include "fft.h"


static
double complex polar_to_complex(const double r, const double theta)
{
    double complex result;
    result = r * cos(theta) + r * sin(theta) * I;

    return result;
}

static
void fft_calc(const int N, const double *x, double complex *X, double complex *P, const int step, const double complex *twids)
{
    int k;
    double complex *S = P + N / 2;
    if (N == 1)
    {
        X[0] = x[0];
        return;
    }

    fft_calc(N / 2, x,      S,   X, 2 * step, twids);
    fft_calc(N / 2, x + step, P,   X, 2 * step, twids);

    for (k = 0; k < N / 2; k++)
    {
        P[k] = P[k] * twids[k * step];
        X[k]     = S[k] + P[k];
        X[k + N / 2] = S[k] - P[k];
    }
}

FFT::FFT(int window_size)
    : FFT_WINDOW(window_size), HALF_FFT(window_size >> 1)
{
    Xt = new double complex[FFT_WINDOW];
    twiddle_factors = new double complex[HALF_FFT];

    for (int k = 0; k < FFT_WINDOW / 2; k++)
    {
        twiddle_factors[k] = polar_to_complex(1.0, 2.0 * M_PI * k / FFT_WINDOW);
    }
}

FFT::~FFT()
{
    delete Xt;
    delete twiddle_factors;
}

void FFT::fft(const double *x, double complex *X)
{
    fft_calc(FFT_WINDOW, x, X, Xt, 1, twiddle_factors);
}
