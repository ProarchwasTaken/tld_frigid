#pragma once

#include <raylib.h>
#include "player.h"


class GoalDoor {
public:
  GoalDoor(int tileX, int tileY);
  ~GoalDoor(); 

  void assignPlayer(Player &player);

  void update();
  void draw();

private:
  Texture2D texture;
  Vector2 position;
  Rectangle rect;

  Player *player;
};
