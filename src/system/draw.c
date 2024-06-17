#include "draw.h"
#include "anime.h"
#include "atlas.h"
#include <raylib.h>

void DrawAtlasSpritePro(char *filename, Rectangle dest, Vector2 origin,
                        float rotation, Color tint, bool flipX)
{
  Game_System *game = getGameSystemInstance();
  AtlasImage image = getAtlasImage(filename);

  if (image.filename != NULL)
  {
    if (flipX)
    {
      image.source.width *= -1;
    }

    DrawTexturePro(game->atlasTexture, image.source, dest, origin, rotation,
                   tint);
    DrawRectangleLines(dest.x, dest.y, dest.width, dest.height, RED);
  }
}

static void drawPlayers(Game_System *game)
{
  Player *players = game->players;
  int player_num = game->numOfPlayers;

  SpriteAnimation idle = createSpriteAnimation(6,
                                               (char *[]){
                                                   "Meow-Knight_Idle_0_0",
                                                   "Meow-Knight_Idle_1_0",
                                                   "Meow-Knight_Idle_2_0",
                                                   "Meow-Knight_Idle_3_0",
                                                   "Meow-Knight_Idle_4_0",
                                                   "Meow-Knight_Idle_5_0",
                                               },
                                               6, true);

  SpriteAnimation walk = createSpriteAnimation(8,
                                               (char *[]){
                                                   "Meow-Knight_Run_0_0",
                                                   "Meow-Knight_Run_1_0",
                                                   "Meow-Knight_Run_2_0",
                                                   "Meow-Knight_Run_3_0",
                                                   "Meow-Knight_Run_4_0",
                                                   "Meow-Knight_Run_5_0",
                                                   "Meow-Knight_Run_6_0",
                                                   "Meow-Knight_Run_7_0",
                                               },
                                               8, true);

  while (player_num--)
  {
    Vector2 pos = players->position;
    bool flip = (players->drawDirection == -1) ? true : false;
    if (players->isMoving)
    {
      drawSpriteAnimationPro(&walk, (Rectangle){pos.x, pos.y, 64, 64},
                             (Vector2){0, 0}, 0, WHITE, flip);
    }
    else
    {
      drawSpriteAnimationPro(&idle, (Rectangle){pos.x, pos.y, 64, 64},
                             (Vector2){0, 0}, 0, WHITE, flip);
    }

    players++;
  }

  disposeSpriteAnimation(&idle);
  disposeSpriteAnimation(&walk);
}

static void drawEnemies(Game_System *game)
{
  Enemy *enemies = game->enemies;
  int enemy_num = game->num_of_enemies;

  if (enemies == NULL)
    return;

  while (enemy_num--)
  {

    char *frames[4];
    if (enemies->type == E_CIVILIAN)
    {
      frames[0] = "vampire_1";
      frames[1] = "vampire_2";
      frames[2] = "vampire_3";
      frames[3] = "vampire_4";
    }
    else if (enemies->type == E_FARMER)
    {
      frames[0] = "slime_1_0";
      frames[1] = "slime_1_1";
      frames[2] = "slime_1_2";
      frames[3] = "slime_1_3";
    }

    SpriteAnimation idle = createSpriteAnimation(4, frames, 6, true);

    Vector2 pos = enemies->position;
    bool flip = (enemies->drawDirection == -1) ? true : false;
    if (enemies->isMoving)
    {
      // TODO: Change this to the correct animation
      //  drawSpriteAnimationPro(&walk, (Rectangle){pos.x, pos.y, 64, 64},
      //                         (Vector2){0, 0}, 0, WHITE, flip);
    }
    else
    {
      // printf("pos.x: %f, pos.y: %f\n", pos.x, pos.y);
      drawSpriteAnimationPro(&idle, (Rectangle){pos.x, pos.y, 64, 64},
                             (Vector2){0, 0}, 0, WHITE, flip);
    }

    disposeSpriteAnimation(&idle);
    enemies++;
  }

  // disposeSpriteAnimation(&walk);
}

static void drawBullets(Game_System *game)
{
  int x = 320, y = 96;
  int bulletNum = game->num_of_bullets;
  Bullet *bullets = game->bullets;

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

  while (bulletNum--)
  {
    Vector2 pos = bullets->position;
    Rectangle dest = {pos.x, pos.y, 32, 32};
    drawSpriteAnimationPro(&fireAnime, dest, (Vector2){0, 0}, 0, WHITE, false);
    bullets->position.x += bullets->bulletSpeed * cos(bullets->angle * DEG2RAD);
    bullets->position.y += bullets->bulletSpeed * sin(bullets->angle * DEG2RAD);
    bullets++;
  }

  disposeSpriteAnimation(&fireAnime);
}
static bool checkCollision(Rectangle rect1, Rectangle rect2)
{
  // collision x-axis?
  bool collisionX = rect1.x + rect1.width >= rect2.x &&
                    rect2.x + rect2.width >= rect1.x;
  // collision y-axis?
  bool collisionY = rect1.y + rect1.height >= rect2.y &&
                    rect2.y + rect2.height >= rect1.y;
  // collision only if on both axes
  return collisionX && collisionY;
}

static void bulletCollision(Game_System *game)
{
  for (int i = 0; i < game->num_of_bullets; i++)
  {
    for (int j = 0; j < game->num_of_enemies; j++)
    {
      if (checkCollision((Rectangle){game->bullets[i].position.x, game->bullets[i].position.y, game->bullets[i].body.width, game->bullets[i].body.height},
                         (Rectangle){game->enemies[j].position.x, game->enemies[j].position.y, game->enemies[j].body.width, game->enemies[j].body.height}))
      {
        game->bullets[i] = game->bullets[game->num_of_bullets - 1];
        game->num_of_bullets--;
        
        if (game->enemies[j].health > 0)
          game->enemies[j].health -= game->bullets[i].bulletDamage;
        else
          game->enemies[j].health = 0;

        // WTF???? Who wrote this shit??
        if (game->enemies[j].health == 0)
        {
          game->enemies[j] = game->enemies[game->num_of_enemies - 1];
          game->num_of_enemies--;
        }
      }
    }
  }
}

void drawScene()
{
  Game_System *gameSystemInstance = getGameSystemInstance();
  BeginDrawing();
  ClearBackground(GetColor(0x052c46ff));

  drawPlayers(gameSystemInstance);

  // TODO: Delete Me later
  // Example for using atlas
  // DrawAtlasSpritePro("vampire_1", (Rectangle){0, 0, 64, 64}, (Vector2){0, 0},
  // 0, WHITE, false);

  // TODO: Delete Me later
  bulletCollision(gameSystemInstance);

  drawEnemies(gameSystemInstance);

  drawBullets(gameSystemInstance);

  EndDrawing();
}
