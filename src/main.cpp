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

  void checkInput();
  void update();
  void draw();

  void loadLevel(); 
  void checkTile(int tileX, int tileY);
  void cleanup();
};


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
    game.checkInput();
    game.update();

    BeginTextureMode(canvas);
    {
      ClearBackground(COLOR::LIGHT);
      game.draw();
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


void Game::checkInput() {
  if (IsKeyPressed(KEY_R)) {
    loadLevel();
  }
  player->keyPressed();
  player->keyReleased();
}


void Game::update() {
  player->update();
}


void Game::draw() {
  for (Rectangle ground : level_geometry) {
    DrawRectangleRec(ground, COLOR::DARK);
  }

  player->draw();
}


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

