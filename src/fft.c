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

int main() {
  pi = atan2f(1, 1) * 4;
  /* printf("%.10f\n", pi); */
  /* size_t n = 4096 * 2; */
  size_t n = 32;
  float in[n];
  float complex out[n];

  for (size_t i = 0; i < n; ++i) {
    float t = (float)i / n;
    in[i] = cosf(2 * pi * t * 1) + sinf(2 * pi * t * 2);
    // We are mixing up 1hz & 3hz
  }
  // Faster fourier transform (we only need to iterate half of the freq.)

  fft(in, 1, out, n);
  /* dft(in, out, n); */

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

// Continue-->  2:13:38 - Music Visualizer (Fast Fourier Transform)
