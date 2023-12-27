#include <stdio.h>
#include <raylib.h>

void callback(void *bufferData, unsigned int frames)
{
  (void) bufferData;
  printf("callback(frames = %u)\n", frames);
}

int main (void) {
  InitWindow(800, 600, "musicviz");
  SetTargetFPS(60);
  InitAudioDevice();

  Music music = LoadMusicStream("Starship.ogg");

  PlayMusicStream(music);
  SetMusicVolume(music, 0.4f);
  AttachAudioStreamProcessor(music.stream, callback);
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
    ClearBackground(BLUE);
    EndDrawing();
  }
  return 0;
}

// https://www.youtube.com/watch?v=Xdbk1Pr5WXU&list=PLpM-Dvs8t0Vak1rrE2NJn8XYEJ5M7-BqT&index=1
