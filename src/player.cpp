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

  jump_time = 0.25;
  last_jumped = 0.0;

  x_accel_seconds = 1;
  x_accel_rate = 0.01;

  y_accel_seconds = 1;
  y_accel_rate = 0.01;
}

void Player::update() {
  movement();
  applyGravity();
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
      break;
    }
  }
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

void Player::keyPressed() {
  if (IsKeyPressed(KEY_RIGHT)) {
    target_velocityX += max_speed;
  }
  if (IsKeyPressed(KEY_LEFT)) {
    target_velocityX -= max_speed;
  }
}

void Player::keyReleased() {
  if (IsKeyReleased(KEY_RIGHT)) {
    target_velocityX -= max_speed;
  }
  if (IsKeyReleased(KEY_LEFT)) {
    target_velocityX += max_speed;
  }
}

void Player::draw() {
  DrawRectangleRec(rect, COLOR::DARK);
}
