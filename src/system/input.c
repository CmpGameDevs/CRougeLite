#include "input.h"
#include "../CRougeLite.h"
#include "../game/weapon.h"
#include <raymath.h>

static void mouseEventHandler();

void handleInput() { 
  mouseEventHandler(); 
}

static void mouseEventHandler() {
  int selected_player = 0;
  Player *player = ((gameState->players) + selected_player);

  Vector2 srcPos = {(double)player->object.transform.position.x +
                        player->object.collider.width / 2 - 16,
                    (double)player->object.transform.position.y +
                        player->object.collider.height / 2 - 16};

  Vector2 mousePos = GetMousePosition();

  mousePos.x -= 16;
  mousePos.y -= 16;

  if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    player->fire = 1;
  }
  if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
    player->fire = 0;
  }
  float deltaTime = GetFrameTime(); // Get time in seconds for one frame

  Weapon *weapon = (player->inventory.weapons + player->inventory.currentWeapon);
  if (weapon->type == RANGED_WEAPON) {
    updateRangedWeapon(weapon, player->fire, player->ID, srcPos, mousePos, deltaTime);
  } else if (weapon->type == MELEE_WEAPON) {
    updateMeleeWeapon(weapon, player->fire, player->ID, srcPos, mousePos, deltaTime);
  }
}
