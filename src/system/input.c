#include "input.h"
#include "../CRougeLite.h"
#include "../game/weapon.h"
#include <raymath.h>

static void mouseEventHandler();

void handleInput() { mouseEventHandler(); }

static int getClickedEnemy(Vector2 mousePos) {

  for (int i = 0; i < gameState->numOfEnemies; i++) {
    Enemy *enemy = (gameState->enemies + i);
    Rectangle bounds = enemy->object.collider.bounds;
    if (CheckCollisionPointRec(mousePos, bounds)) {
      return i;
    }
  }
  return -1;
}

static void mouseEventHandler() {
  if (IsKeyPressed(KEY_TAB)) {
    gameState->settings.showDebugMenu ^= 1;
  }

  if (!gameState->isGameStarted) {
    bool isGameStarted = IsKeyPressed(KEY_ENTER);
    gameState->isGameStarted = isGameStarted;
    return;
  }

  int selected_player = 0;
  Player *player = ((gameState->players) + selected_player);

  Vector2 srcPos = {(double)player->object.transform.position.x +
                        player->object.collider.bounds.width / 2,
                    (double)player->object.transform.position.y +
                        player->object.collider.bounds.height / 2};

  Vector2 mousePos = GetMousePosition();
  mousePos = GetScreenToWorld2D(mousePos, gameState->camera);

  mousePos.x -= 16;
  mousePos.y -= 16;

  if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    player->fire = 1;
  }
  if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
    player->fire = 0;
  }
  float deltaTime = GetFrameTime(); // Get time in seconds for one frame

  Weapon *weapon =
      (player->inventory.weapons + player->inventory.currentWeapon);
  if (weapon->type == RANGED_WEAPON) {
    if (weapon->weapon.ranged.bulletInfo.isTracking) {
      weapon->weapon.ranged.bulletInfo.enemyID = getClickedEnemy(mousePos);
    }
    updateRangedWeapon(weapon, player->fire, player->ID, srcPos, mousePos,
                       deltaTime);
  } else if (weapon->type == MELEE_WEAPON) {
    updateMeleeWeapon(weapon, player->fire, player->ID, srcPos, mousePos,
                      deltaTime);
  }
}
