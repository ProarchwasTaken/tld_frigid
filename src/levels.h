#pragma once 
#include "constants.h"

#define LEVEL_COUNT 1
#define SOLID_TILE 1
#define PLAYER_SPAWNPOINT 2

/* This array is for storing every level in the game and their layout.
 * 0 = Nothing
 * 1 = Wall/Ground
 * 2 = Player spawnpoint.*/
extern int LEVELS[LEVEL_COUNT][TILE::ROWS][TILE::COLUMNS];

