#include <raylib.h>
#include "constants.h"
#include "player.h"
#include "goal_door.h"


// GoalDoor class
GoalDoor::GoalDoor(int tileX, int tileY) {
  texture = LoadTexture("sprites/goal_door.png");
  tileY--;
  position = {tileX * TILE::SIZE - 1, tileY * TILE::SIZE - 1};
  rect = (Rectangle){position.x, position.y, 6, 9};
}

GoalDoor::~GoalDoor() {
  UnloadTexture(texture);
}

void GoalDoor::assignPlayer(Player &player) {
  this->player = &player;
}

void GoalDoor::draw() {
  DrawTextureV(texture, position, WHITE);
}
