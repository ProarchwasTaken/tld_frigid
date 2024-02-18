#include <iostream>
#include <array>
#include <raylib.h>
#include <raymath.h>
#include "constants.h"
#include "player.h"

using std::array;

Player::Player(int tileX, int tileY, array<Rectangle, 2> level_geometry) {
  tileY -= 1;
  rect = (Rectangle){tileX * TILE::SIZE, tileY * TILE::SIZE, 4, 8};
  this->level_geometry = &level_geometry;

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

void Player::gravityAcceleration() {
  if (jumping) {
    return;
  }

  if (velocity_y != max_falling_speed) {
    velocity_y += y_accel_rate * (GetFrameTime() * FRAMERATE::TARGET);
    velocity_y = Clamp(velocity_y, -jump_speed, max_falling_speed);
  } 
}

void Player::initiateJump() {
  std::cout << "Player is now jumping.\n";
  jumping = true;
  on_ground = false;

  velocity_y = -jump_speed;

  last_jumped = GetTime();
}

void Player::jumpSequence() {
  float time_elapsed = GetTime() - last_jumped;

  bool passed_minimum_time = time_elapsed >= min_jump_time;
  bool passed_maximum_time = time_elapsed >= max_jump_time;

  if (passed_minimum_time && holding_jumpKey == false) {
    std::cout << "Player is no longer jumping.\n";
    jumping = false;
    return;
  }
  else if (passed_maximum_time) {
    std::cout << "Player is no longer jumping.\n";
    jumping = false;
    return;
  }
}

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
