#include <raylib.h>
#include "constants.h"
#include "goal_door.h"


// GoalDoor Constructor. Called on initialization.
GoalDoor::GoalDoor(int tileX, int tileY) {
  texture = LoadTexture("sprites/goal_door.png");
  tileY--;
  position = {tileX * TILE::SIZE - 1, tileY * TILE::SIZE - 1};
  rect = (Rectangle){position.x, position.y, 6, 9};
}

// GoalDoor Destructor
GoalDoor::~GoalDoor() {
  UnloadTexture(texture);
}

void GoalDoor::draw() {
  DrawTextureV(texture, position, WHITE);
}
