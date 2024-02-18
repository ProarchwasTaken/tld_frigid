#include <iostream>
#include <raylib.h>
#include "constants.h"
#include "player.h"


int main() {
  InitWindow(WINDOW::WIDTH, WINDOW::HEIGHT, "Nokia 3310 Jam 6");
  SetTargetFPS(FRAMERATE::MAX);

  auto canvas = LoadRenderTexture(CANVAS::WIDTH, CANVAS::HEIGHT);
  Rectangle source = (Rectangle){0, 0, CANVAS::WIDTH, -CANVAS::HEIGHT};
  Rectangle dest = (Rectangle){0, 0, WINDOW::WIDTH, WINDOW::HEIGHT};
  Vector2 origin = {0, 0};

  Rectangle level[2] = {
    (Rectangle){0, 40, 42, 8},
    (Rectangle){54, 40, 30, 8}
  };

  auto player = Player(4, 9);
  
  std::cout << "Everything seems to be good to go!." << "\n";

  while (WindowShouldClose() == false) {

    BeginTextureMode(canvas);
    ClearBackground(COLOR::LIGHT);

    for (Rectangle ground : level) {
      DrawRectangleRec(ground, COLOR::DARK);
    }

    player.draw();

    EndTextureMode();

    BeginDrawing();
    DrawTexturePro(canvas.texture, source, dest, origin, 0, WHITE);
    EndDrawing();
  }

  UnloadRenderTexture(canvas);
  return 0;
}
