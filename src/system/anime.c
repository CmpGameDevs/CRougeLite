/***************************************************************
 *
 *     █████╗ ███╗   ██╗██╗███╗   ███╗███████╗
 *    ██╔══██╗████╗  ██║██║████╗ ████║██╔════╝
 *    ███████║██╔██╗ ██║██║██╔████╔██║█████╗
 *    ██╔══██║██║╚██╗██║██║██║╚██╔╝██║██╔══╝
 *    ██║  ██║██║ ╚████║██║██║ ╚═╝ ██║███████╗
 *    ╚═╝  ╚═╝╚═╝  ╚═══╝╚═╝╚═╝     ╚═╝╚══════╝
 *
 *     Animations and effects for the game
 *     Authored by Amir Kedis
 *
 *
 **************************************************************/

#include "anime.h"
#include "atlas.h"
#include "draw.h"
#include <raylib.h>

Rectangle getSrcRect(Animator *animator)
{
  SpriteAnimation *anim = &animator->animations[animator->currentState];
  return getAtlasImage(anim->frameNames[anim->currentFrame]).source;
}

void setState(Animator *animator, int state)
{
  if (animator->currentState == state)
    return;

  animator->currentState = state;
  animator->animations[state].currentFrame = 0;
  animator->animations[state].frameCount = 0;
  animator->animations[state].isFinished = false;
  animator->isFinished = false;
}

void updateAnimator(Animator *animator)
{
  SpriteAnimation *anim = &animator->animations[animator->currentState];

  if (anim->isFinished && !anim->isLooping)
  {
    animator->isFinished = true;
    return;
  }

  if (anim->isLooping)
    animator->isFinished = true;
  else
    animator->isFinished = false;

  anim->frameCount++;
  double divisor = 60.0f / anim->fps;
  anim->currentFrame = anim->frameCount / divisor;

  if (anim->currentFrame >= anim->numOfFrames)
  {
    anim->currentFrame = 0;
    anim->frameCount = 0;
    anim->isFinished = true;
  }

  if (anim->isFinished && anim->isLooping)
  {
    anim->currentFrame = 0;
    anim->frameCount = 0;
    anim->isFinished = false;
  }
}

void drawAnimator(Animator *animator, CTransform *transform, Color tint,
                  bool flipX)
{
  SpriteAnimation *anim = &animator->animations[animator->currentState];
  Rectangle src = getSrcRect(animator);
  DrawAtlasSpritePro(anim->frameNames[anim->currentFrame],
                     (Rectangle){transform->position.x, transform->position.y,
                                 src.width * transform->scale.x,
                                 src.height * transform->scale.y},
                     (Vector2){0, 0}, 0, tint, flipX);
}
