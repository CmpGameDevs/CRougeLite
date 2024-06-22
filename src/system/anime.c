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

// void drawSpriteAnimationPro(SpriteAnimation *anim, Rectangle dest,
//                             Vector2 origin, float rotation, Color tint,
//                             bool flipX) {
//   int index = (int)(GetTime() * anim->fps) % anim->numOfFrames;
//   DrawAtlasSpritePro(anim->frameNames[index], dest, origin, rotation, tint,
//                      flipX);
// }

Rectangle getSrcRect(Animator *animator) {
  SpriteAnimation *anim = &animator->animatinos[animator->currentState];
  return getAtlasImage(anim->frameNames[anim->currentFrame]).source;
}
