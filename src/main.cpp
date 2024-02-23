#include <iostream>
#include <list>
#include <memory>
#include <raylib.h>
#include "constants.h"
#include "levels.h"
#include "player.h"
#include "goal_door.h"

using std::unique_ptr, std::list, std::make_unique;


/* The Game struct handles a multitude of things. Which vary from checking
 * for player input, updating and drawing the game's elements to loading
 * the game's levels, and cleanup.*/
struct Game {
  int state = TITLE;
  int current_level = 0;

  unique_ptr<Player> player;
  unique_ptr<GoalDoor> goal_door;
  list<Rectangle> level_geometry;

  void checkInput();
  void update();
  void draw();

  void nextLevel();
  void loadLevel(); 
  void checkTile(int tileX, int tileY);
  void cleanup();
};


void drawTitleScreen() {
  DrawText("Press [Z]", 18, 36, 10, COLOR::DARK);
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
  std::cout << "Game directory: "<< GetApplicationDirectory() << "\n";

  while (WindowShouldClose() == false) {
    switch (game.state) {
      default:
        break;

      case TITLE: {
        if (IsKeyPressed(KEY_Z)) {
          game.state = GAME;
          game.loadLevel();
        }
        break;
      }

      case GAME: {
        game.checkInput();
        game.update();
        break;
      }
    }

    BeginTextureMode(canvas);
    {
      ClearBackground(COLOR::LIGHT);

      switch (game.state) {
        default:
          break;
          
        case TITLE: {
          drawTitleScreen();
          break;
        }
        
        case GAME: {
          game.draw();
          break;
        }
      }
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
    std::cout << "Reloading current level.\n";
    loadLevel();
  }
  player->keyPressed();
  player->keyReleased();
}


void Game::update() {
  player->update();

  if (CheckCollisionRecs(player->rect, goal_door->rect)) {
    std::cout << "Player has reached the goal door.\n";
    nextLevel();
  }
}


void Game::draw() {
  for (Rectangle ground : level_geometry) {
    DrawRectangleRec(ground, COLOR::DARK);
  }

  goal_door->draw();
  player->draw();
}


void Game::nextLevel() {
  current_level++;

  if (current_level == LEVEL_COUNT) {
    std::cout << "No availiable levels left. Returning to title screen\n";
    state = TITLE;
    current_level = 0;

    cleanup();
    return;
  }

  loadLevel();
}


/* The game loads a level in accordance to the value of the current_level
 * integer. Usually called when the game starts, when it's time to load
 * the next level after the player completes the current one.*/
void Game::loadLevel() {
  std::cout << "================================\n";
  std::cout << "Loading level: " << current_level << "\n";
  cleanup();

  std::cout << "Proceeding to search through level array to create " <<
    "game's elements.\n";
  for (int y = 0; y < TILE::ROWS; y++) {
    for (int x = 0; x < TILE::COLUMNS; x++) {
      Game::checkTile(x, y);
    }
  }
  std::cout << "Number of solid tiles: " << level_geometry.size() << "\n";

  std::cout << "Assigning address of the level's geometry to player.\n"; 
  player->assignLevelGeometry(level_geometry);

  std::cout << "Assigning Player address to GoalDoor\n";
  goal_door->assignPlayer(*player);

  std::cout << "Level has successfully loaded!\n";
  std::cout << "================================\n";
}


/* Checks a certain value in the two dimensional level array and does
 * something depending on that value.*/
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
      std::cout << "Initializing Player object at: (" << tileX << ", " <<
        tileY << ")\n";
      player = make_unique<Player>(tileX, tileY);
      break;
    }
    case GOAL_DOOR: {
      std::cout << "Initializing GoalDoor object at: (" << tileX << ", " 
        << tileY << ")\n";
      goal_door = make_unique<GoalDoor>(tileX, tileY);
      break;
    }
  }

}


/* For freeing up the game's memory to prevent memory leaks. Usually
 * called when the game loads a new level or when the program is about
 * to close.*/
void Game::cleanup() {
  std::cout << "Freeing up used memory...\n";
  if (player != nullptr) {
    player.reset();
    std::cout << "Freed up memory used by the Player object.\n";
  }
  if (goal_door != nullptr) {
    goal_door.reset();
    std::cout << "Freed up memory used by the GoalDoor object.\n";
  }

  level_geometry.clear();
}

