#include <raylib.h>
#include "levels.h"
#include "solid_tile.h"
#include "ice_spike.h"
#include "player.h"


IceSpike::IceSpike(int tileX, int tileY): 
  Tile(tileX, tileY) {
  tile_type = ICE_SPIKE;

  texture = LoadTexture("sprites/spikes.png");
  position = (Vector2){rect.x, rect.y};
}

IceSpike::~IceSpike() {
  UnloadTexture(texture);
}

void IceSpike::assignPlayer(Player &player) {
  this->player = &player;
}

void IceSpike::draw() {
  DrawTextureV(texture, position, WHITE);
}
