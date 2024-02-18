#include <raylib.h>
#include "constants.h"
#include "player.h"


Player::Player(int tile_x, int tile_y) {
  tile_y -= 1;
  rect = (Rectangle){tile_x * TILE::SIZE, tile_y * TILE::SIZE, 4, 8};
  spawn_point = (Vector2){rect.x, rect.y};
}

void Player::draw() {
  DrawRectangleRec(rect, COLOR::DARK);
}
