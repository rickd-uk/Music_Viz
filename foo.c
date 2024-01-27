#include <complex.h>
#include <math.h>
#include <stdio.h>

#define N 30

float in[N];
float complex out[N];

void dft(float in[], float complex out[], size_t n) {

  // Slow fourier transform
  for (size_t f = 0; f < n; ++f) {
    out[f] = 0;
    for (size_t i = 0; i < n; ++i) {
      float t = (float)i / n;

      // use Euler's formula
      out[f] += in[i] * cexp(2 * I * M_PI * f * t);
      /* out_sin[f] += in[i] * sinf(2 * pi * f * t); */
      /* out_cos[f] += in[i] * cosf(2 * pi * f * t); */
    }
  }
}

int main(void) {
  float f = 1.0f;
  for (size_t i = 0; i < N; ++i) {
    float t = (float)i / N;
    in[i] = sinf(2 * M_PI * f * t);
  }

  dft(in, out, N);

  float mx = 0;
  for (size_t i = 0; i < N; ++i) {
    float a = cabsf(out[i]);
    if (mx < a)
      mx = a;
  }

  // N / 2 (ignore half the freq. because they are mirrored)
  for (size_t i = 0; i < N / 2; ++i) {
    float a = cabsf(out[i]);
    float t = a / mx;
    for (size_t j = 0; j < t * N; ++j) {
      printf("*");
    }
    printf("\n");
  }
  for (size_t k = 0; k < 2; ++k) {
    for (size_t i = 0; i < N; ++i) {
      float t = (in[i] + 1) / 2 * N;
      for (size_t j = 0; j < t; ++j) {
        printf(" ");
      }
      printf("*\n");
    }
  }
  return 0;
}
