#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include "constants.h"
#include "player.h"
#include "solid_tile.h"

// Player Constructor. Called on initialization. 
Player::Player(int tileX, int tileY) {
  tileY -= 1;
  rect = (Rectangle){tileX * TILE::SIZE, tileY * TILE::SIZE + 1, 4, 7};
  texture = LoadTexture("sprites/player.png");

  spawn_point = (Vector2){rect.x, rect.y};
  position = (Vector2){rect.x, rect.y};

  max_speed = 0.5;
  max_falling_speed = 0.5;

  jump_speed = 0.5;

  min_jump_time = 0.01;
  max_jump_time = 0.25;

  coyote_time = 0.15;

  x_accel_seconds = 1;
  x_accel_rate = 0.015;

  y_accel_seconds = 1;
  y_accel_rate = 0.05;

  starting_temperature = 30;
  current_temperature = starting_temperature; 

  default_tick_seconds = 1;
  icespike_devisor = 4;
  tick_timestamp = GetTime();
}

// Player Destructor.
Player::~Player() {
  UnloadTexture(texture);
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
  position = {rect.x, rect.y};

  if (movingWhileNoKeysDown()) {
    target_velocityX = 0;
  }
  
  if (rect.y >= CANVAS::HEIGHT) {
    std::cout << "The player has fell out of the level.\n";
    softReset();
  }
  else {
    temperatureHandling();
  }
}

/* For assigning the reference to the level_geometry of the current level
 * which will be used to check for collisions. Usually called after the
 * player has been initiated, and the level have been fully loaded.*/
void Player::assignLevelGeometry(list<Tile*> &level_geometry) {
  this->level_geometry = &level_geometry;
}

/* Returns true if the player somehow not moving even when no keys are
 * pressed. In such a scenario, the player's target_velocity will be set
 * to 0.*/
bool Player::movingWhileNoKeysDown() {
  bool noKeysPressed = IsKeyUp(KEY_RIGHT) && IsKeyUp(KEY_LEFT);

  if (target_velocityX != 0 && noKeysPressed) {
    return true;
  }
  else {
    return false;
  }
}

/* For performing a "soft reset" of the player by reseting most of their
 * parameters and sending them back to their spawn point. Usually called
 * when the player falls down a bottomless pit or their temperature hits
 * 0.*/
void Player::softReset() {
  std::cout << "Now Performing a soft reset.\n";
  rect.x = spawn_point.x;
  rect.y = spawn_point.y;
  position = spawn_point;

  velocity_x = 0;
  velocity_y = 0;

  current_temperature = starting_temperature;
  tick_timestamp = GetTime();
  jumping = false;
  touchingIceSpikes = false;
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

  for (Tile *tile : *level_geometry) {
    if (CheckCollisionRecs(rect, tile->rect) == true) {
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
 * and save a timestamp of the last time the player was grounded..*/
void Player::applyGravity() {
  gravityAcceleration();

  if (velocity_y == 0) {
    return;
  }

  rect.y += velocity_y * (GetFrameTime() * FRAMERATE::TARGET);

  for (Tile *tile : *level_geometry) {
    if (CheckCollisionRecs(rect, tile->rect) == true) {
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

  if (on_ground == true) {
    on_ground = false;
    last_grounded = GetTime();
  }

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

/* Pretty much decides if the player should be able to jump based on a
 * number of conditions. Returns true if only one of these conditions
 * are met. Called every time the player tries to jump.*/
bool Player::canJump() {
  float time_elapsed = GetTime() - last_grounded;
  bool coyote_time_over = time_elapsed >= coyote_time;

  if (on_ground || coyote_time_over == false) {
    return true;
  }
  else {
    return false;
  }
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

/* Call once every frame. Handles almost everything that has something
 * to do with temperature like performing a soft reset on the player
 * of their temperature hits 0.*/
void Player::temperatureHandling() {
  float tick_seconds;
  if (touchingIceSpikes) {
    tick_seconds = default_tick_seconds / icespike_devisor;
  }
  else {
    tick_seconds = default_tick_seconds;
  }

  float time_elapsed = GetTime() - tick_timestamp;
  bool enough_time_passed = time_elapsed >= tick_seconds;

  if (enough_time_passed) {
    --current_temperature;

    if (current_temperature == 0){
      std::cout << "The player's temperature has hit 0.\n";
      softReset();
    }
    else {
      tick_timestamp = GetTime();
    }
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
    if (canJump()) {
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
  DrawRectangle(0, 0, 12, 9, COLOR::DARK);
  DrawText(TextFormat("%02i", current_temperature), 0, 0, 10, COLOR::LIGHT);
  DrawTextureV(texture, position, WHITE);
}
