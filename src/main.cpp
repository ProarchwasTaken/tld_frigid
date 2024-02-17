#include <iostream>
#include <raylib.h>
#include "constants.h"


int main() {
  InitWindow(WINDOW::WIDTH, WINDOW::HEIGHT, "Nokia 3310 Jam 6");
  SetTargetFPS(FRAMERATE::MAX);

  auto canvas = LoadRenderTexture(CANVAS::WIDTH, CANVAS::HEIGHT);
  Rectangle source = (Rectangle){0, 0, CANVAS::WIDTH, -CANVAS::HEIGHT};
  Rectangle dest = (Rectangle){0, 0, WINDOW::WIDTH, WINDOW::HEIGHT};
  Vector2 origin = {0, 0};
  
  std::cout << "Everything seems to be good to go!." << "\n";

  while (WindowShouldClose() == false) {

    BeginTextureMode(canvas);
    ClearBackground(COLOR::LIGHT);
    EndTextureMode();

    BeginDrawing();
    DrawTexturePro(canvas.texture, source, dest, origin, 0, WHITE);
    EndDrawing();
  }

  UnloadRenderTexture(canvas);
  return 0;
}
