#pragma once

#include <list>
#include <raylib.h>

using std::list;

/* The player character is a special game object is does stuff based on 
 * the input of the user. It is capable of moving but with a lot of 
 * slipperiness, and it's also able to jump.*/
class Player {
public:
  Player(int tileX, int tileY);
  ~Player();

  Vector2 position;
  Rectangle rect;

  void assignLevelGeometry(list<Rectangle> &level_geometry);

  void update();
  void draw();

  void keyPressed();
  void keyReleased();

private:
  Texture2D texture;

  Vector2 spawn_point;
  list<Rectangle> *level_geometry;

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
  float coyote_time;
  float last_grounded;

  bool jumping = false;
  bool holding_jumpKey = false;
  float jump_speed;

  float max_jump_time;
  float min_jump_time;
  float last_jumped = 0.0;

  int starting_temperature;
  int current_temperature;


  void movement();
  void acceleration();

  void initiateJump();
  void jumpSequence();
  bool canJump();

  bool movingWhileNoKeysDown();

  void applyGravity();
  void gravityAcceleration();
};
