#include <cstddef>
#include <iostream>
#include <list>
#include <memory>
#include <raylib.h>
#include "constants.h"
#include "levels.h"
#include "player.h"

using std::unique_ptr, std::list, std::make_unique;


struct Game {
  int current_level = 0;

  unique_ptr<Player> player;
  list<Rectangle> level_geometry;

  void loadLevel(); 
  void checkTile(int tileX, int tileY);
  void cleanup();
};

void Game::loadLevel() {
  cleanup();

  for (int y = 0; y < TILE::ROWS; y++) {
    for (int x = 0; x < TILE::COLUMNS; x++) {
      Game::checkTile(x, y);
    }
  }

  player->assignLevelGeometry(level_geometry);
}

void Game::checkTile(int tileX, int tileY) {
  switch (LEVELS[current_level][tileY][tileX]) {
    default:
      break;

    case SOLID_TILE: {
      float x = tileX * TILE::SIZE;
      float y = tileY * TILE::SIZE;

      level_geometry.push_front(
        (Rectangle){x , y, TILE::SIZE, TILE::SIZE}
      );
      
      break;
    }

    case PLAYER_SPAWNPOINT: {
      player = make_unique<Player>(tileX, tileY);
      break;
    }
  }

}

void Game::cleanup() {
  if (player != NULL) {
    player.reset();
  }

  level_geometry.clear();
}


int main() {
  InitWindow(WINDOW::WIDTH, WINDOW::HEIGHT, "Nokia 3310 Jam 6");
  SetTargetFPS(FRAMERATE::MAX);

  auto canvas = LoadRenderTexture(CANVAS::WIDTH, CANVAS::HEIGHT);
  Rectangle source = (Rectangle){0, 0, CANVAS::WIDTH, -CANVAS::HEIGHT};
  Rectangle dest = (Rectangle){0, 0, WINDOW::WIDTH, WINDOW::HEIGHT};
  Vector2 origin = {0, 0};

  Game game;
  unique_ptr<Player> &player = game.player;

  game.loadLevel();
  
  std::cout << "Everything seems to be good to go!." << "\n";

  while (WindowShouldClose() == false) {
    if (IsKeyPressed(KEY_R)) {
      game.loadLevel();
    }

    player->keyPressed();
    player->keyReleased();

    player->update();

    BeginTextureMode(canvas);
    {
      ClearBackground(COLOR::LIGHT);

      for (Rectangle ground : game.level_geometry) {
        DrawRectangleRec(ground, COLOR::DARK);
      }

      player->draw();
    }
    EndTextureMode();

    BeginDrawing();
    {
      DrawTexturePro(canvas.texture, source, dest, origin, 0, WHITE);
    }
    EndDrawing();
  }

  game.cleanup();
  UnloadRenderTexture(canvas);
  std::cout << "Thanks for playing!\n";
  return 0;
}
