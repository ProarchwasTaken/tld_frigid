#include <raylib.h>
#include <raymath.h>
#include "constants.h"
#include "player.h"


Player::Player(int tile_x, int tile_y) {
  tile_y -= 1;
  rect = (Rectangle){tile_x * TILE::SIZE, tile_y * TILE::SIZE, 4, 8};

  spawn_point = (Vector2){rect.x, rect.y};

  max_speed = 0.5;
  max_falling_speed = 1;

  jump_time = 0.25;
  last_jumped = 0.0;

  x_accel_seconds = 1;
  x_accel_rate = 0.01;

  y_accel_seconds = 1;
}

void Player::update() {
  movement();
}

void Player::movement() {
  acceleration();

  rect.x += velocity_x * (GetFrameTime() * FRAMERATE::TARGET);
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

void Player::keyPressed() {
  if (IsKeyPressed(KEY_LEFT)) {
    target_velocityX += max_speed;
  }
  if (IsKeyPressed(KEY_RIGHT)) {
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
