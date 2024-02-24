#pragma once

#include "constants.h"

#define LEVEL_COUNT 2
#define SOLID_TILE 1
#define PLAYER_SPAWNPOINT 2
#define GOAL_DOOR 3
#define ICE_SPIKE 4

/* This array is for storing every level in the game and their layout.
 * 0 = Nothing
 * 1 = Wall/Ground
 * 2 = Player spawnpoint.
 * 3 = Goal Door*/
extern int LEVELS[LEVEL_COUNT][TILE::ROWS][TILE::COLUMNS];

