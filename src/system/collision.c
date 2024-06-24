#include "collision.h"
#include <raylib.h>

static bool CheckAABBCollision(const Rectangle a, const Rectangle b)
{
  return (a.x < b.x + b.width &&
          a.x + a.width > b.x &&
          a.y < b.y + b.height &&
          a.y + a.height > b.y);
}

void resolveCollision(GameObject *a, GameObject *b)
{

}

void updateGameObjectPosition(GameObject *a, Vector2 newPosition)
{
  
}