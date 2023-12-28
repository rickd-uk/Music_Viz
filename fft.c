#include <complex.h>
#include <math.h>
#include <stdio.h>

float pi;

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
#if 1
  for (size_t f = 0; f < n / 2; ++f) {
    out[f] = 0;
    out[f + n / 2] = 0;

    // even indices
    for (size_t i = 0; i < n; i += 2) {
      float t = (float)i / n;
      float complex v = in[i] * cexp(2 * I * pi * t * f);
      out[f] += v;
      out[f + n / 2] += v;
    }
    // odd indices
    for (size_t i = 1; i < n; i += 2) {
      float t = (float)i / n;
      float complex v = in[i] * cexp(2 * I * pi * t * f);
      out[f] += v;
      out[f + n / 2] -= v;
    }
  }

#else
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

#endif
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

