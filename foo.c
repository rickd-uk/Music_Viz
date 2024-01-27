#include <math.h>
#include <stdio.h>

#define N 30

float in[N];

int main(void) {
  for (size_t i = 0; i < N; ++i) {
    float t = (float)i / N;
    in[i] = sinf(2 * M_PI * t);
  }
  for (size_t i = 0; i < N; ++i) {
    for (size_t j = 0; j < (in[i] + 1) / 2 * N; ++j) {
      printf(" ");
    }
    printf("*\n");
  }
  return 0;
}
