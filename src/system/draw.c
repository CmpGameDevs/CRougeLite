#include "draw.h"

#include "../game/combatAction.h"
#include "../game/debugMenu.h"
#include "../game/enemy.h"
#include "../game/player.h"
#include "atlas.h"
#include "map.h"
#include <raylib.h>

void DrawAtlasSpritePro(char *filename, Rectangle dest, Vector2 origin,
                        float rotation, Color tint, bool flipX) {
  AtlasImage image = getAtlasImage(filename);

  if (image.filename != NULL) {
    if (flipX) {
      image.source.width *= -1;
    }

    if (gameState->settings.showTextures)
      DrawTexturePro(gameState->atlasTexture, image.source, dest, origin,
                     rotation, tint);
    if (gameState->settings.showTexturesBounds)
      DrawRectangleLines(dest.x, dest.y, dest.width, dest.height,
                         GetColor(0x8B4000AA));
  }
}

void drawColidors() {
  // draw map colliders
  // TODO: ya akram

  // draw players colliders
  Player *players = gameState->players;
  for (int i = 0; i < gameState->numOfPlayers; i++) {
    Player *player = &players[i];
    DrawRectangleLines(player->object.transform.position.x,
                       player->object.transform.position.y,
                       player->object.collider.width,
                       player->object.collider.height, RED);
  }

  // draw enemies colliders
  Enemy *enemies = gameState->enemies;
  for (int i = 0; i < gameState->numOfEnemies; i++) {
    Enemy *enemy = &enemies[i];
    DrawRectangleLines(
        enemy->object.transform.position.x, enemy->object.transform.position.y,
        enemy->object.collider.width, enemy->object.collider.height, RED);
  }

  // draw bullet actions colliders
  CombatAction *actions = gameState->combatActions;
  for (int i = 0; i < gameState->numOfCombatActions; i++) {
    CombatAction *action = &actions[i];
    DrawRectangleLines(action->action.bullet.transform.position.x,
                       action->action.bullet.transform.position.y,
                       action->action.bullet.bulletInfo.collider.width,
                       action->action.bullet.bulletInfo.collider.height, RED);
  }
}

void drawScene() {
  BeginDrawing();

  ClearBackground(GetColor(0x25131aff));

  BeginMode2D(gameState->camera);
  drawMap();

  drawPlayers();
  drawEnemies();

  drawCombatActions();

  Vector2 cur = GetMousePosition();
  Vector2 worldPos = GetScreenToWorld2D(cur, gameState->camera);
  DrawCircle(worldPos.x, worldPos.y, 5, RED);

  if (gameState->settings.showColliders)
    drawColidors();

  EndMode2D();

  if (gameState->settings.showDebugMenu)
    drawDebugMenu();

  if (gameState->settings.showFPS)
    DrawFPS(10, 10);

  EndDrawing();
}
