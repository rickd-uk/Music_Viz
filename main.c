#include <stdio.h>
#include <raylib.h>

int main (void) {
  InitWindow(800, 600, "musicviz");
  InitAudioDevice();

  Music music = LoadMusicStream("Starship.ogg");

    PlayMusicStream(music);
  while (!WindowShouldClose()) {
    UpdateMusicStream(music);

    BeginDrawing();
    ClearBackground(BLUE);
    EndDrawing();
  }
  return 0;
}
