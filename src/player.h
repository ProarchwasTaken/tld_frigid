#pragma once

#include "raylib.h"

class Player {
  Rectangle rect;

  float velocity_x = false;
  float velocity_y = false;

  bool on_ground = false;

public:
  Vector2 origin;
};
