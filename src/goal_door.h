#pragma once

#include <raylib.h>


class GoalDoor {
public:
  GoalDoor(int tileX, int tileY);
  ~GoalDoor();  

  void update();
  void draw();

private:
  Texture2D texture;
  Rectangle rect;
  Vector2 position;
};
