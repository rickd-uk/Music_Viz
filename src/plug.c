#include "plug.h"
#include <assert.h>
#include <math.h>
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

float in[N];
float _Complex out[N];

typedef struct {
  Music music;
} Plug;

Plug *plug;

void fft(float in[], size_t stride, float complex out[], size_t n) {
  assert(n > 0);

  if (n == 1) {
    out[0] = in[0];
    return;
  }
  // test
  fft(in, stride * 2, out, n / 2);
  fft(in + stride, stride * 2, out + n / 2, n / 2);

  for (size_t k = 0; k < n / 2; ++k) {
    float t = (float)k / n;
    float complex v = cexp(-2 * I * PI * t) * out[k + n / 2];
    float complex e = out[k];
    out[k] = e + v;
    out[k + n / 2] = e - v;
  }
}

float amp(float complex z) {
  float a = fabsf(crealf(z));
  float b = fabsf(cimagf(z));
  if (a < b)
    return b;
  return a;
}

void callback(void *bufferData, unsigned int frame) {

  // Frame *fs = bufferData;
  float(*fs)[plug->music.stream.channels] = bufferData;

  for (size_t i = 0; i < frame; ++i) {
    memmove(in, in + 1, (N - 1) * sizeof(in[0]));
    in[N - 1] = fs[i][0];
  }
}

void plug_init(void) {

  plug = malloc(sizeof(*plug));
  assert(plug != NULL && "Memory allocation failed!");
  memset(plug, 0, sizeof(*plug));

}

Plug *plug_pre_reload(void) {
  if (IsMusicReady(plug->music)) {
    DetachAudioStreamProcessor(plug->music.stream, callback);
  }
  return plug;
}

void plug_post_reload(Plug *prev) {
  plug = prev;
  if (IsMusicReady(plug->music)) {
    AttachAudioStreamProcessor(plug->music.stream, callback);
  }
}

int q = 0;
void plug_update(void) {

  q += 1;
  if (IsMusicReady(plug->music)) {
    UpdateMusicStream(plug->music);
  }
  if (IsKeyPressed(KEY_SPACE)) {
    if (IsMusicReady(plug->music)) {
      if (IsMusicStreamPlaying(plug->music)) {
        PauseMusicStream(plug->music);
      } else {
        ResumeMusicStream(plug->music);
      }
    }
  }

  if (IsKeyPressed(KEY_Q)) {
    if (IsMusicReady(plug->music)) {
      StopMusicStream(plug->music);
      PlayMusicStream(plug->music);
    }
  }

  if (IsFileDropped()) {
    FilePathList droppedFiles = LoadDroppedFiles();

    if (droppedFiles.count > 0) {
      const char *file_path = droppedFiles.paths[0];
      if (IsMusicReady(plug->music)) {
        StopMusicStream(plug->music);
        UnloadMusicStream(plug->music);
      }
      plug->music = LoadMusicStream(file_path);

      if (IsMusicReady(plug->music)) {
        printf("music.frameCount = %u\n", plug->music.frameCount);
        printf("music.stream.sampleRate = %u\n", plug->music.stream.sampleRate);
        printf("music.stream.sampleSize = %u\n", plug->music.stream.sampleSize);
        printf("music.stream.channels = %u\n", plug->music.stream.channels);

        SetMusicVolume(plug->music, 0.5f);
        AttachAudioStreamProcessor(plug->music.stream, callback);
        PlayMusicStream(plug->music);
      }

      UnloadDroppedFiles(droppedFiles);
    }
  }
  int w = GetRenderWidth();
  int h = GetRenderHeight();

  BeginDrawing();
  ClearBackground(BLACK);

  if (q % 3 == 0)
    fft(in, 1, out, N);

  float max_amp = 0.0f;
  for (size_t i = 0; i < N; ++i) {
    float a = amp(out[i]);
    if (max_amp < a)
      max_amp = a;
  }

  float step = 1.06;
  size_t m = 0;
  for (float f = 20.0f; (size_t)f < N; f *= step) {
    m += 1;
  }

  float cell_width = (float)w / m;
  m = 0;
  for (float f = 20.0f; (size_t)f < N; f *= step) {
    float f1 = f * step;
    float a = 0.0f;
    for (size_t q = (size_t)f; q < N && q < (size_t)f1; ++q) {
      a += amp(out[q]);
    }
    a /= (size_t)f1 - (size_t)f + 1;
    float t = a / max_amp;
    /* DrawRectangle(m * cell_width, h / 2, cell_width, h / 2 * t, GREEN); */
    // DrawRectangle(m * cell_width, h / 2 - h / 2 * t, cell_width, h / 2 * t, YELLOW);
    /* DrawCircle(m * cell_width, h / 2 - h / 2 * t, h / 2 * t, BLUE); */
    DrawCircle(m * cell_width, h / 2, h / 2 * t, BLUE);
    m += 1;
  }
  /* for (size_t i = 0; i < global_frames_count; ++i) { */
  /*   float t = global_frames[i].left; */
  /*   if (t > 0) { */
  /*     DrawRectangle(i * cell_width, h / 2.0f - h / 2.0f * t, 1, h / 2.0f * t, RED); */
  /*   } else { */
  /*     DrawRectangle(i * cell_width, h / 2.0f, 1, h / 2.0f * t, RED); */
  /*   } */
  /* } */
  /* if (global_frames_count > 0) exit(1); */
  EndDrawing();
}
