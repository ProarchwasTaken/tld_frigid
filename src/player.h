#pragma once

#include "raylib.h"

class Player {
  Rectangle rect;
  Vector2 spawn_point;

  float velocity_x = 0;
  float velocity_y = 0;

  bool on_ground = false;

public:
  Player(int tile_x, int tile_y);
  void update();
  void draw();
};
