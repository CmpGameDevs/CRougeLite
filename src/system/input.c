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

  if (IsKeyDown(KEY_ESCAPE))
  {
    game->finished = true;
  }
  if (IsKeyDown(KEY_W))
  {
    pos->y -= speed;
    *direction = UP;
  }
  if (IsKeyDown(KEY_S))
  {
    pos->y += speed;
    *direction = DOWN;
  }
  if (IsKeyDown(KEY_A))
  {
    pos->x -= speed;
    *direction = LEFT;
    player->drawDirection = -1;
  }
  if (IsKeyDown(KEY_D))
  {
    pos->x += speed;
    *direction = RIGHT;
    player->drawDirection = 1;
  }
}

static void mouseEventHandler(Game_System *game)
{
  static float elapsedTime = 0.0f;
  int selected_player = 0;
  Player *player = ((game->players) + selected_player);
  Vector2 mousePos = GetMousePosition();
  if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
  {
    player->fire = 1;
  }
  if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
  {
    player->fire = 0;
  }
  if (player->fire == 1)
  {
    float deltaTime = GetFrameTime(); // Get time in seconds for one frame

    // Check if the interval has passed
    if (elapsedTime >= player->fireRate || elapsedTime == 0.0f)
    {
      initBullet(player->weapon, selected_player, player->position,mousePos);
      // Reset the timer
      elapsedTime = 0.0f;
    }
    elapsedTime += deltaTime;
  }
  else
    elapsedTime = 0.0f;
}
