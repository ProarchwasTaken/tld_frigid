#include <raylib.h>
#include "levels.h"
#include "solid_tile.h"
#include "ice_spike.h"


IceSpike::IceSpike(int tileX, int tileY): 
  Tile(tileX, tileY) {
  tile_type = ICE_SPIKE;

  texture = LoadTexture("sprites/spikes.png");
  position = (Vector2){rect.x, rect.y};

  detection_rect = (Rectangle){rect.x - 1, rect.y - 1, 6, 6};
}

IceSpike::~IceSpike() {
  UnloadTexture(texture);
}

void IceSpike::draw() {
  DrawTextureV(texture, position, WHITE);
}
