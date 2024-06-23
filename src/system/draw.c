#include "draw.h"

#include "../game/combatAction.h"
#include "../game/debugMenu.h"
#include "../game/enemy.h"
#include "../game/player.h"
#include "atlas.h"
#include "map.h"

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
    DrawRectangleLines(dest.x, dest.y, dest.width, dest.height, RED);
  }
}

void drawScene() {
  BeginDrawing();
  ClearBackground(GetColor(0x052c46ff));
  drawMap();

  drawPlayers();
  drawEnemies();

  drawCombatActions();
  if (gameState->settings.showDebugMenu)
    drawDebugMenu();

  EndDrawing();
}
