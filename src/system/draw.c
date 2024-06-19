#include "draw.h"
#include "../game/player.h"
#include "../game/combatAction.h"
#include "anime.h"
#include "atlas.h"
#include <raylib.h>

void DrawAtlasSpritePro(char *filename, Rectangle dest, Vector2 origin,
                        float rotation, Color tint, bool flipX) {
  AtlasImage image = getAtlasImage(filename);

  if (image.filename != NULL) {
    if (flipX) {
      image.source.width *= -1;
    }

    DrawTexturePro(gameState->atlasTexture, image.source, dest, origin,
                   rotation, tint);
    DrawRectangleLines(dest.x, dest.y, dest.width, dest.height, RED);
  }
}

static void drawEnemies() {
  Enemy *enemies = gameState->enemies;
  int enemy_num = gameState->numOfEnemies;

  if (enemies == NULL)
    return;

  while (enemy_num--) {

    char *frames[4];
    if (enemies->type == E_CIVILIAN) {
      frames[0] = "vampire_1";
      frames[1] = "vampire_2";
      frames[2] = "vampire_3";
      frames[3] = "vampire_4";
    } else if (enemies->type == E_FARMER) {
      frames[0] = "slime_1_0";
      frames[1] = "slime_1_1";
      frames[2] = "slime_1_2";
      frames[3] = "slime_1_3";
    }

    SpriteAnimation idle = createSpriteAnimation(4, frames, 6, true);

    Vector2 pos = enemies->object.transform.position;
    bool flip = (enemies->drawDirection == -1) ? true : false;
    drawSpriteAnimationPro(&idle, (Rectangle){pos.x, pos.y, 64, 64},
                           (Vector2){0, 0}, 0, WHITE, flip);

    disposeSpriteAnimation(&idle);
    enemies++;
  }

  // disposeSpriteAnimation(&walk);
}



void drawScene() {
  BeginDrawing();
  ClearBackground(GetColor(0x052c46ff));

  drawPlayers();

  drawCombatActions();
  // TODO: Delete Me later
  // Example for using atlas
  // DrawAtlasSpritePro("vampire_1", (Rectangle){0, 0, 64, 64}, (Vector2){0, 0},
  // 0, WHITE, false);

  // TODO: Delete Me later
  drawEnemies();


  EndDrawing();
}
