#include "input.h"
#include "../CRougeLite.h"
#include <raymath.h>

static void mouseEventHandler();

void handleInput() { 
  mouseEventHandler(); 
}

static void mouseEventHandler() {
  int selected_player = 0;
  Player *player = ((gameState->players) + selected_player);

  Vector2 srcPos = {(double)player->object.rigidBody.position.x +
                        player->object.collider.width / 2 - 16,
                    (double)player->object.rigidBody.position.y +
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
  float *reloadTime = &(player->inventory.weapons->weapon.ranged.stats.lastUseTime);

  float cooldown = player->inventory.weapons->weapon.ranged.stats.cooldown;

  int *ammo = &(player->inventory.weapons->weapon.ranged.ammo);

  if (player->fire == 1 && *ammo > 0 && player->inventory.weapons->type == RANGED_WEAPON &&
      *reloadTime <= 0.0f) {

    initBullet(player->ID, &(player->inventory.weapons->weapon.ranged.bulletInfo), srcPos,
               mousePos);
    *ammo -= 1;
    *reloadTime = cooldown;
  }
  if (*reloadTime > 0.0f)
    *reloadTime -= deltaTime;
}
