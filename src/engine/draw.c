#include "draw.h"
#include "../CRougeLite.h"
#include <raylib.h>

static drawPlayers(Game_System *game) {
  Player *players = game->players;
  int player_num = game->num_of_players;
  Rectangle source = {0, 0, 32, 32};
  while (player_num--) {
    Vector2 pos = players->position;
    Rectangle dest = {pos.x, pos.y, 64, 64};
    DrawTexturePro(players->texture, source, dest, (Vector2){0, 0}, 0, WHITE);
    players++;
  }
}

void drawScene() {
  Game_System *gameSystemInstance = getGameSystemInstance();
  BeginDrawing();
  ClearBackground(GetColor(0x052c46ff));
  
  drawPlayers(gameSystemInstance);

  EndDrawing();
}
