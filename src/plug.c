#include "plug.h"
#include <assert.h>
#include <math.h>
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FONT_SIZE 39

float in[N];
float _Complex out[N];

typedef struct {
  Music music;
  Font font;
  bool error;
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
  // float a = fabsf(crealf(z));
  // float b = fabsf(cimagf(z));
  // if (a < b)
  //   return b;
  // return a;
  return cabsf(z);
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

  plug->font = LoadFontEx("./fonts/RubikBurned-Regular.ttf", FONT_SIZE, NULL, 0);
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
        plug->error = false;
        printf("music.frameCount = %u\n", plug->music.frameCount);
        printf("music.stream.sampleRate = %u\n", plug->music.stream.sampleRate);
        printf("music.stream.sampleSize = %u\n", plug->music.stream.sampleSize);
        printf("music.stream.channels = %u\n", plug->music.stream.channels);

        SetMusicVolume(plug->music, 0.5f);
        AttachAudioStreamProcessor(plug->music.stream, callback);
        PlayMusicStream(plug->music);
      } else {
        plug->error = true;
      }

      UnloadDroppedFiles(droppedFiles);
    }
  }
  int w = GetRenderWidth();
  int h = GetRenderHeight();

  BeginDrawing();
  ClearBackground(BLACK);

  if (IsMusicReady(plug->music)) {

    fft(in, 1, out, N);

    float max_amp = 0.0f;
    for (size_t i = 0; i < N / 2; ++i) {
      float a = amp(out[i]);
      if (max_amp < a)
        max_amp = a;
    }

    float step = 1.06;
    float lowf = 1.0f;
    size_t m = 0;
    for (float f = lowf; (size_t)f < N / 2; f = ceilf(f * step)) {
      m += 1;
    }

    float cell_width = (float)w / m;
    m = 0;
    for (float f = lowf; (size_t)f < N / 2; f = ceilf(f * step)) {
      float f1 = ceilf(f * step);
      float a = 0.0f;
      for (size_t q = (size_t)f; q < N / 2 && q < (size_t)f1; ++q) {
        float b = amp(out[q]);
        if (b > a)
          a = b;
      }
      float t = a / max_amp;
      DrawRectangle(m * cell_width, h / 2 - h / 2 * t, cell_width, h / 2 * t, GREEN);
      // DrawCircle(m * cell_width, h / 2, h / 2 * t, BLUE);
      m += 1;
    }
  } else {
    const char *label;
    Color color;
    if (plug->error) {
      label = "It must be a valid audio file";
      color = RED;
    } else {
      label = "Drag & drop here!";
      color = WHITE;
    }
    Vector2 size = MeasureTextEx(plug->font, label, plug->font.baseSize, 0);
    Vector2 position = {
        (float)w / 2 - size.x / 2,
        (float)h / 2 - size.y / 2,
    };

    DrawTextEx(plug->font, label, position, plug->font.baseSize, 0, color);
  }
  EndDrawing();
}
