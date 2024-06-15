#include "draw.h"
#include "../CRougeLite.h"
#include <raylib.h>

void drawScene() {
  BeginDrawing();
  ClearBackground(GetColor(0x052c46ff));
  Rectangle source = {0, 0, 32, 32};
  Rectangle dest = {player.x, player.y, 64, 64};

  DrawTexturePro(player.texture, source, dest, (Vector2){0, 0}, 0, WHITE);

  Rectangle bulletSource = {0, 16 * 3, 16, 16};
  Rectangle bulletDest = {bullet.x, bullet.y, 32, 32};
  if (bullet.health > 0)
    DrawTexturePro(bullet.texture, bulletSource, bulletDest, (Vector2){0, 0}, 0,
                   WHITE);

  EndDrawing();
}
