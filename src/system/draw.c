#include "draw.h"
#include <raylib.h>

static void drawPlayers(Game_System *game) {
  Player *players = game->players;
  int player_num = game->num_of_players;

  Rectangle source = {0, 0, players->drawDirection * 16, 16};
  while (player_num--) {
    Vector2 pos = players->position;
    Rectangle dest = {pos.x, pos.y, 32, 32};
    DrawTexturePro(players->texture, source, dest, (Vector2){0, 0}, 0, WHITE);
    players++;
  }
}

static void drawBullets(Game_System *game) {
  int x = 320, y = 96;
  int bulletNum = game->num_of_bullets;
  Bullet *bullets = game->bullets;
  while (bulletNum--) {
    Rectangle source = {x, y, 16, 16};
    Vector2 pos = bullets->position;
    Rectangle dest = {pos.x, pos.y, 16, 16};
    DrawTexturePro(game->bulletTexture, source, dest, (Vector2){0, 0},
                   bullets->angle, WHITE);
    bullets->position.x += bullets->bulletSpeed * cos(bullets->angle * DEG2RAD);
    bullets->position.y += bullets->bulletSpeed * sin(bullets->angle * DEG2RAD);
    bullets++;
  }
}

void drawScene() {
  Game_System *gameSystemInstance = getGameSystemInstance();
  BeginDrawing();
  ClearBackground(GetColor(0x052c46ff));

  // TODO: remove this later just here for testing
  DrawTexture(gameSystemInstance->atlasTexture, 0, 0, WHITE);

  drawPlayers(gameSystemInstance);
  drawBullets(gameSystemInstance);

  EndDrawing();
}
