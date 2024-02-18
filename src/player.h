#pragma once

#include "raylib.h"

/* The player character is a special game object is does stuff based on 
 * the input of the user. It is capable of moving but with a lot of 
 * slipperiness, and it's also able to jump.*/
class Player {
  Rectangle rect;
  Vector2 spawn_point;

  float velocity_x = 0;
  float velocity_y = 0;
  float target_velocityX = 0;

  float x_accel_seconds;
  float x_accel_rate;

  float y_accel_seconds;
  double y_accel_percentage = 0.0;

  float max_speed;
  float max_falling_speed;

  bool on_ground = false;

  bool jumping = false;
  float jump_time;
  float last_jumped;

public:
  Player(int tile_x, int tile_y);
  void update();
  void draw();

  void movement();
  void acceleration();

  void keyPressed();
  void keyReleased();
};
