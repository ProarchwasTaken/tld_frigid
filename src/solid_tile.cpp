#include <raylib.h>
#include "levels.h"
#include "solid_tile.h"


Tile::Tile(int tileX, int tileY) {
  tile_type = SOLID_TILE;

  rect = (Rectangle){ 
    tileX * TILE::SIZE, tileY * TILE::SIZE, TILE::SIZE, TILE::SIZE
  };
}

void Tile::draw() {
  DrawRectangleRec(rect, COLOR::DARK);
}
