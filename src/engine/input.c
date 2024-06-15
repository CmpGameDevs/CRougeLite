#include "input.h"
#include "../CRougeLite.h"

void handleInput() {
  if (IsKeyPressed(KEY_ESCAPE)) {
    CloseWindow();
  }
  if (IsKeyDown(KEY_W)) {
    player.y -= PLAYER_SPEED;
  }
  if (IsKeyDown(KEY_S)) {
    player.y += PLAYER_SPEED;
  }
  if (IsKeyDown(KEY_A)) {
    player.x -= PLAYER_SPEED;
  }
  if (IsKeyDown(KEY_D)) {
    player.x += PLAYER_SPEED;
  }
  if (IsKeyDown(KEY_SPACE)) {
    state.isFiring = true;
  } else {
    state.isFiring = false;
  }
}
