#include "draw.h"
#include "../game/player.h"
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

    Vector2 pos = enemies->object.rigidBody.position;
    bool flip = (enemies->drawDirection == -1) ? true : false;
    drawSpriteAnimationPro(&idle, (Rectangle){pos.x, pos.y, 64, 64},
                           (Vector2){0, 0}, 0, WHITE, flip);

    disposeSpriteAnimation(&idle);
    enemies++;
  }

  // disposeSpriteAnimation(&walk);
}

static bool checkCollision(Rectangle rect1, Rectangle rect2) {
  // collision x-axis?
  bool collisionX =
      rect1.x + rect1.width >= rect2.x && rect2.x + rect2.width >= rect1.x;
  // collision y-axis?
  bool collisionY =
      rect1.y + rect1.height >= rect2.y && rect2.y + rect2.height >= rect1.y;
  // collision only if on both axes
  return collisionX && collisionY;
}

static int bulletCollision(CombatAction *combatActions) {
  Bullet *bullet = &combatActions->action.bullet;
  for (int j = 0; j < gameState->numOfEnemies; j++) {
    Enemy *enemy = &gameState->enemies[j];

    if (checkCollision((Rectangle){bullet->bulletInfo.rigidBody.position.x,
                                   bullet->bulletInfo.rigidBody.position.y,
                                   bullet->bulletInfo.collider.width,
                                   bullet->bulletInfo.collider.height},
                       (Rectangle){enemy->object.rigidBody.position.x,
                                   enemy->object.rigidBody.position.y,
                                   enemy->object.collider.width,
                                   enemy->object.collider.height})) {
      *combatActions =
          gameState->combatActions[gameState->numOfCombatActions - 1];
      gameState->numOfCombatActions--;
      enemy->stats.health.currentHealth -= bullet->bulletInfo.bulletDamage;
      if (enemy->stats.health.currentHealth <= 0) {
        gameState->enemies[j] = gameState->enemies[gameState->numOfEnemies - 1];
        gameState->numOfEnemies--;
      }
      return 1;
    }
  }
  return 0;
}
static void drawBullets() {
  int x = 320, y = 96;
  int actions = gameState->numOfCombatActions;
  CombatAction *combatActions = gameState->combatActions;

  SpriteAnimation fireAnime = createSpriteAnimation(6,
                                                    (char *[]){
                                                        "fire_1_0_0",
                                                        "fire_1_0_1",
                                                        "fire_1_0_2",
                                                        "fire_1_0_3",
                                                        "fire_1_0_4",
                                                        "fire_1_0_5",
                                                    },
                                                    12, true);

  while (actions--) {
    if (combatActions->type == ACTION_BULLET) {
      Bullet *bullets = &combatActions->action.bullet;
      Vector2 *pos = &bullets->bulletInfo.rigidBody.position;
      Rectangle dest = {pos->x, pos->y, bullets->bulletInfo.collider.width,
                        bullets->bulletInfo.collider.height};
      drawSpriteAnimationPro(&fireAnime, dest, (Vector2){0, 0}, 0, WHITE,
                             false);
      pos->x +=
          bullets->bulletInfo.bulletSpeed * cos(combatActions->angle * DEG2RAD);
      pos->y +=
          bullets->bulletInfo.bulletSpeed * sin(combatActions->angle * DEG2RAD);
      combatActions -= bulletCollision(combatActions);
    }
    combatActions++;
  }
  disposeSpriteAnimation(&fireAnime);
}

void drawScene() {
  BeginDrawing();
  ClearBackground(GetColor(0x052c46ff));

  drawPlayers();

  // TODO: Delete Me later
  // Example for using atlas
  // DrawAtlasSpritePro("vampire_1", (Rectangle){0, 0, 64, 64}, (Vector2){0, 0},
  // 0, WHITE, false);

  // TODO: Delete Me later
  drawEnemies();

  drawBullets();

  EndDrawing();
}
