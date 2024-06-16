#include "input.h"
#include "../CRougeLite.h"

static void keyboardEventHandler(Game_System *game);
static void mouseEventHandler(Game_System *game);

void handleInput()
{
  Game_System *gameSystemInstance = getGameSystemInstance();
  keyboardEventHandler(gameSystemInstance);

  mouseEventHandler(gameSystemInstance);
}

static void keyboardEventHandler(Game_System *game)
{
  int selected_player = 0;
  Player *player = ((game->players) + selected_player);
  Vector2 *pos = &(player->position);
  DIRECTIONS *direction = &(player->direction);
  int speed = player->speed;

  player->isMoving = false;

  if (IsKeyDown(KEY_ESCAPE))
  {
    game->finished = true;
  }
  if (IsKeyDown(KEY_W))
  {
    pos->y -= speed;
    *direction = UP;
    player->isMoving = true;
  }
  if (IsKeyDown(KEY_S))
  {
    pos->y += speed;
    *direction = DOWN;
    player->isMoving = true;
  }
  if (IsKeyDown(KEY_A))
  {
    pos->x -= speed;
    *direction = LEFT;
    player->isMoving = true;
    player->drawDirection = -1;
  }
  if (IsKeyDown(KEY_D))
  {
    pos->x += speed;
    *direction = RIGHT;
    player->isMoving = true;
    player->drawDirection = 1;
  }
}

static void mouseEventHandler(Game_System *game)
{
  int selected_player = 0;
  Player *player = ((game->players) + selected_player);

  Vector2 srcPos = {(double)player->position.x + player->body.width / 2, (double)player->position.y + player->body.height / 2};


  Vector2 mousePos = GetMousePosition();
  if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
  {
    player->fire = 1;
  }
  if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
  {
    player->fire = 0;
  }
  float deltaTime = GetFrameTime(); // Get time in seconds for one frame
  if (player->fire == 1 && player->reloadTime <= 0.0f)
  {
      initBullet(player->weapon, selected_player, srcPos, mousePos);
      player->reloadTime = player->fireRate;
    }
  if(player->reloadTime > 0.0f)
    player->reloadTime -= deltaTime;
}
