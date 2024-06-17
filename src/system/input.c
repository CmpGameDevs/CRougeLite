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

// 1,2,5,9 theses number if you sum any two numbers you will get a unique number not in the list and not equal the sum of any other two numbers same for 3 numbers.
// I choose these numbers to make it easy to know the angle of movement without making alot of conditions on key press.
// 1,2,5,9
// w,a,s,d

static void keyboardEventHandler(Game_System *game)
{
  int selected_player = 0;
  Player *player = ((game->players) + selected_player);
  Vector2 *pos = &(player->position);
  DIRECTIONS *direction = &(player->direction);
  int speed = player->speed;

  static int angles[18]; // the size is the sum of the 4 numbers for fast retrieval of the angle.
  memset(angles, -1, sizeof(angles));
  angles[W + D + A] = angles[W] = -90;
  angles[W + S + A] = angles[A] = 180;
  angles[A + S + D] = angles[S] = 90;
  angles[W + S + D] = angles[D] = 0;
  angles[W + A] = -135;
  angles[W + D] = -45;
  angles[S + D] = 45;
  angles[S + A] = 135;

  player->isMoving = false;

  int sum = 0;

  if (IsKeyDown(KEY_ESCAPE))
  {
    game->finished = true;
  }
  if (IsKeyDown(KEY_W))
  {
    sum += 1;
    *direction = UP;
  }
  if (IsKeyDown(KEY_S))
  {
    sum += 5;
    *direction = DOWN;
  }
  if (IsKeyDown(KEY_A))
  {
    sum += 2;
    *direction = LEFT;
    player->drawDirection = -1;
  }
  if (IsKeyDown(KEY_D))
  {
    sum += 9;
    *direction = RIGHT;
    player->drawDirection = 1;
  }
  if (angles[sum] == -1)
    return;
  player->isMoving = true;
  pos->x += speed * cos(angles[sum] * DEG2RAD);
  pos->y += speed * sin(angles[sum] * DEG2RAD);
}

static void mouseEventHandler(Game_System *game)
{
  int selected_player = 0;
  Player *player = ((game->players) + selected_player);

  Vector2 srcPos = {(double)player->position.x + player->body.width / 2 - 16, (double)player->position.y + player->body.height / 2 - 16};

  Vector2 mousePos = GetMousePosition();

  mousePos.x -= 16;
  mousePos.y -= 16; 

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
    initBullet(player->weapon, selected_player, (RigidBody2d){32, 32}, srcPos, mousePos);
    player->reloadTime = player->fireRate;
  }
  if (player->reloadTime > 0.0f)
    player->reloadTime -= deltaTime;
}
