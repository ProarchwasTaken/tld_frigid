#pragma once

#include <raylib.h>


class GoalDoor {
public:
  GoalDoor(int tileX, int tileY);
  ~GoalDoor(); 

  Rectangle rect;

  void draw();

private:
  Texture2D texture;
  Vector2 position;
};
