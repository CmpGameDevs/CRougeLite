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

void drawColliders() {
  // draw map colliders
  // TODO: ya akram

  // draw players colliders
  Player *players = gameState->players;
  for (int i = 0; i < gameState->numOfPlayers; i++) {
    Rectangle bounds = players[i].object.collider.bounds;
    DrawRectangleLines(bounds.x,
                       bounds.y,
                       bounds.width,
                       bounds.height, RED);
  }

  // draw enemies colliders
  Enemy *enemies = gameState->enemies;
  for (int i = 0; i < gameState->numOfEnemies; i++) {
    Rectangle bounds = enemies[i].object.collider.bounds;
    DrawRectangleLines(bounds.x,
                       bounds.y,
                       bounds.width,
                       bounds.height, RED);
  }

  // draw bullet actions colliders
  CombatAction *actions = gameState->combatActions;
  for (int i = 0; i < gameState->numOfCombatActions; i++) {
    GameObject *object = NULL;
    switch (actions[i].type) {
      case ACTION_BULLET:
        object = &(actions[i].action.bullet.bulletInfo.object);
        break;
      case ACTION_SLASH:
        object = &(actions[i].action.slash.slashInfo.object);
        break;
      default:
        break;
    }
    Rectangle bounds = object->collider.bounds;
    DrawRectangleLines(bounds.x,
                       bounds.y,
                       bounds.width,
                       bounds.height, RED);
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
    drawColliders();

  EndMode2D();

  if (gameState->settings.showDebugMenu)
    drawDebugMenu();

  if (gameState->settings.showFPS)
    DrawFPS(10, 10);

  EndDrawing();
}
