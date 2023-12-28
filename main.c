#include <assert.h>
#include <raylib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARRAY_LEN(xs) sizeof(xs) / sizeof(xs[0])

uint32_t global_frames[1024];
size_t global_frames_count = 0;

void callback(void *bufferData, unsigned int frames) {
  if (frames > ARRAY_LEN(global_frames)) {
    frames = ARRAY_LEN(global_frames);
  }
  memcpy(global_frames, bufferData, sizeof(uint32_t) * frames);
  global_frames_count = frames;
}

int main(void) {
  InitWindow(800, 600, "musicviz");
  SetTargetFPS(60);
  InitAudioDevice();

  Music music = LoadMusicStream("Starship.ogg");
  assert(music.stream.sampleSize == 16);
  assert(music.stream.channels == 2);

  PlayMusicStream(music);
  SetMusicVolume(music, 0.4f);
  AttachAudioStreamProcessor(music.stream, callback);

  printf("music.stream.sampleRate = %u\n", music.stream.sampleRate);
  printf("music.stream.sampleSize = %u\n", music.stream.sampleSize);
  printf("music.stream.channels = %u\n", music.stream.channels);
  while (!WindowShouldClose()) {
    UpdateMusicStream(music);

    if (IsKeyPressed(KEY_SPACE)) {
      if (IsMusicStreamPlaying(music)) {
        PauseMusicStream(music);
      } else {
        ResumeMusicStream(music);
      }
    }
    BeginDrawing();
    ClearBackground(BLACK);
    float cell_width = (float)GetRenderWidth() / global_frames_count;
    for (size_t i = 0; i < global_frames_count; ++i) {
      int16_t sample = *(int16_t *)&global_frames[i];
      printf("%d ", sample);
    }
    printf("\n");
    /* if (global_frames_count > 0) exit(1); */
    EndDrawing();
  }
  return 0;
}

// https://www.youtube.com/watch?v=Xdbk1Pr5WXU&list=PLpM-Dvs8t0Vak1rrE2NJn8XYEJ5M7-BqT&index=1
