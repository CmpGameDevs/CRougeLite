#include "input.h"
#include "../CRougeLite.h"

static void keyboardEventHandler(Game_System *game);
static void mouseEventHandler(Game_System *game);

void handleInput() {
  Game_System *gameSystemInstance = getGameSystemInstance();
  keyboardEventHandler(gameSystemInstance);

  mouseEventHandler(gameSystemInstance);
}

static void keyboardEventHandler(Game_System *game) {
  int selected_player = 0;
  Player *player = ((game->players) + selected_player);
  Vector2 *pos = &(player->position);
  int speed = player->speed;
  
  if (IsKeyDown(KEY_ESCAPE)) {
    game->finished = true;
  }
  if (IsKeyDown(KEY_W)) {
    pos->y -= speed;
  }
  if (IsKeyDown(KEY_S)) {
    pos->y += speed;
  }
  if (IsKeyDown(KEY_A)) {
    pos->x -= speed;
  }
  if (IsKeyDown(KEY_D)) {
    pos->x += speed;
  }
}

static void mouseEventHandler(Game_System *game) {
  Vector2 mousePos = GetMousePosition();
}

