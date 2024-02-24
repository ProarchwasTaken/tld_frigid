#pragma once

#include <raylib.h>

class Tile {
public:
  Tile(int tileX, int tileY);
  int tile_type;
  Rectangle rect;

  virtual void draw();
};
