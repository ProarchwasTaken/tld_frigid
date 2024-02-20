#include <raylib.h>
#include <raymath.h>
#include "constants.h"
#include "player.h"

// Player class 
Player::Player(int tileX, int tileY) {
  tileY -= 1;
  rect = (Rectangle){tileX * TILE::SIZE, tileY * TILE::SIZE, 4, 8};

  spawn_point = (Vector2){rect.x, rect.y};

  max_speed = 0.5;
  max_falling_speed = 0.5;

  jump_speed = 0.5;

  min_jump_time = 0.1;
  max_jump_time = 0.25;

  x_accel_seconds = 1;
  x_accel_rate = 0.01;

  y_accel_seconds = 1;
  y_accel_rate = 0.05;
}

/* Is called once every frame. Does a multitude of things like moving the
 * player, applying gravity, and respawning the player at their
 * designated spawn point if they were to fall out of the level.*/
void Player::update() {
  movement();

  if (jumping) {
    jumpSequence();
  }

  applyGravity();

  if (rect.y >= CANVAS::HEIGHT) {
    rect.x = spawn_point.x;
    rect.y = spawn_point.y;
  }
}


void Player::assignLevelGeometry(list<Rectangle> &level_geometry) {
  this->level_geometry = &level_geometry;
}


/* Responsible for moving the player. The method would be returned
 * early if the player isn't moving at all.*/
void Player::movement() {
  acceleration();

  bool playerNotMoving = velocity_x == 0;
  if (playerNotMoving) {
    return;
  }

  rect.x += velocity_x * (GetFrameTime() * FRAMERATE::TARGET);

  for (Rectangle ground : *level_geometry) {
    if (CheckCollisionRecs(rect, ground) == true) {
      rect.x -= velocity_x * (GetFrameTime() * FRAMERATE::TARGET);
      break;
    }
  }
}

/* For slowing incrementing and decrementing the player's velocity to 
 * simulate the effect of acceleration or deceleration.*/
void Player::acceleration() {
  if (velocity_x < target_velocityX) {
    velocity_x += x_accel_rate * (GetFrameTime() * FRAMERATE::TARGET);
    velocity_x = Clamp(velocity_x, velocity_x, target_velocityX);
  }
  else if (velocity_x > target_velocityX) {
    velocity_x -= x_accel_rate * (GetFrameTime() * FRAMERATE::TARGET);
    velocity_x = Clamp(velocity_x, target_velocityX, velocity_x);
  }
}

/* Moves the player down automatically each frame to simulate gravity.
 * Responsible for canceling the player's jump if they were to hit a
 * ceiling. Also Responsible for deciding if the player is on the ground
 * or not.*/
void Player::applyGravity() {
  gravityAcceleration();

  if (velocity_y == 0) {
    return;
  }

  rect.y += velocity_y * (GetFrameTime() * FRAMERATE::TARGET);

  for (Rectangle ground : *level_geometry) {
    if (CheckCollisionRecs(rect, ground) == true) {
      rect.y -= velocity_y * (GetFrameTime() * FRAMERATE::TARGET);
      velocity_y = 0;

      if (jumping) {
        jumping = false;
      }
      else if (on_ground == false) {
        on_ground = true;
      }

      return;
    }
  }

  on_ground = false;
}

/* This is sorta the same thing as the Acceleration method, but for the Y
 * axis. As long as the player's Y velocity is below a certain point, it
 * will always be continually incremented each frame. Unless the player is
 * jumping.*/
void Player::gravityAcceleration() {
  if (jumping) {
    return;
  }

  if (velocity_y != max_falling_speed) {
    velocity_y += y_accel_rate * (GetFrameTime() * FRAMERATE::TARGET);
    velocity_y = Clamp(velocity_y, -jump_speed, max_falling_speed);
  } 
}

/* For kickstarting the player's jumping sequence. Should only be called
 * if the Z key is pressed, and the player is on the ground.*/
void Player::initiateJump() {
  jumping = true;
  on_ground = false;

  velocity_y = -jump_speed;

  last_jumped = GetTime();
}

/* Called once every frame if the player is jumping. This method is for
 * naturally stopping the jumping sequence if certain conditions are met.
 * Once of which being if enough time has passed.*/
void Player::jumpSequence() {
  float time_elapsed = GetTime() - last_jumped;

  bool passed_minimum_time = time_elapsed >= min_jump_time;
  bool passed_maximum_time = time_elapsed >= max_jump_time;

  if (passed_minimum_time && holding_jumpKey == false) {
    jumping = false;
    return;
  }
  else if (passed_maximum_time) {
    jumping = false;
    return;
  }
}

/* For checking if any particular keys are pressed and do something
 * depending on what key it is. Called once every frame.*/
void Player::keyPressed() {
  if (IsKeyPressed(KEY_RIGHT)) {
    target_velocityX += max_speed;
  }
  if (IsKeyPressed(KEY_LEFT)) {
    target_velocityX -= max_speed;
  }

  if (IsKeyPressed(KEY_Z)) {
    if (on_ground) {
      initiateJump();
    }

    holding_jumpKey = true;
  }
}

/* Nearly the same as the other method but for checking if any particular
 * keys are RELEASED.*/
void Player::keyReleased() {
  if (IsKeyReleased(KEY_RIGHT)) {
    target_velocityX -= max_speed;
  }
  if (IsKeyReleased(KEY_LEFT)) {
    target_velocityX += max_speed;
  }

  if (IsKeyReleased(KEY_Z)) {
    holding_jumpKey = false;
  }
}

void Player::draw() {
  DrawRectangleRec(rect, COLOR::DARK);
}
