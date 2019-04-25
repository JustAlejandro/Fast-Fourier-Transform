#ifndef FFT_H
#define FFT_H
#include <complex>
#include <valarray>

typedef std::complex<double> Complex;
typedef std::valarray<Complex> CArray;

extern Complex FFTdata[2048];

void fft(CArray& x);


#endif // !FFT_H