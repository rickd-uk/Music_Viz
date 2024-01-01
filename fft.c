#include <assert.h>
#include <complex.h>
#include <math.h>
#include <stdio.h>
#include <threads.h>

float pi;

void dft(float in[], float complex out[], size_t n) {

  // Slow fourier transform
  for (size_t f = 0; f < n; ++f) {
    out[f] = 0;
    for (size_t i = 0; i < n; ++i) {
      float t = (float)i / n;

      // use Euler's formula
      out[f] += in[i] * cexp(2 * I * pi * f * t);
      /* out_sin[f] += in[i] * sinf(2 * pi * f * t); */
      /* out_cos[f] += in[i] * cosf(2 * pi * f * t); */
    }
  }
}

void fft(float in[], size_t stride, float complex out[], size_t n) {
  assert(n > 0);

  if (n == 1) {
    out[0] = in[0];
    return;
  }

  fft(in, stride * 2, out, n / 2);
  fft(in + stride, stride * 2, out + n / 2, n / 2);

  for (size_t k = 0; k < n / 2; ++k) {
    float t = (float)k / n;
    float complex v = cexp(-2 * I * pi * t) * out[k + n / 2];
    float complex e = out[k];
    out[k] = e + v;
    out[k + n / 2] = e - v;
  }
}

int main() {
  pi = atan2f(1, 1) * 4;
  /* printf("%.10f\n", pi); */
  size_t n = 8;
  float in[n];
  float complex out[n];

  for (size_t i = 0; i < n; ++i) {
    float t = (float)i / n;
    in[i] = cosf(2 * pi * t * 1) + sinf(2 * pi * t * 2);
    // We are mixing up 1hz & 3hz
  }
  // Faster fourier transform (we only need to iterate half of the freq.)

  fft(in, 1, out, n);

  for (size_t f = 0; f < n; ++f) {
    printf("%02zu: %.2f, %.2f\n", f, creal(out[f]), cimag(out[f]));
  }

  return 0;
}
// One period of sine wave is 2*pi
// 1f = 1 hertz
// if out[f] is positive, it means that freq. is present in the input
//  the value of it is the amplitude
// clang -o fft fft.c -lm && ./fft

// Continuee-->  2:13:38 - Music Visualizer (Fast Fourier Transform)
