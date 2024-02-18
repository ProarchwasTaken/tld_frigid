#pragma once

#include "raylib.h"
#include <array>

using std::array;

/* The player character is a special game object is does stuff based on 
 * the input of the user. It is capable of moving but with a lot of 
 * slipperiness, and it's also able to jump.*/
class Player {
  Rectangle rect;
  Vector2 spawn_point;
  array<Rectangle, 2> *level_geometry;

  float velocity_x = 0;
  float velocity_y = 0;
  float target_velocityX = 0;

  float x_accel_seconds;
  float x_accel_rate;

  float y_accel_seconds;
  float y_accel_rate;

  float max_speed;
  float max_falling_speed;

  bool on_ground = false;

  bool jumping = false;
  float jump_speed;
  float jump_time;
  float last_jumped;

public:
  Player(int tileX, int tileY, array<Rectangle, 2> level_geometry);
  void update();
  void draw();

  void movement();
  void acceleration();

  void applyGravity();
  void gravityAcceleration();

  void keyPressed();
  void keyReleased();
};
