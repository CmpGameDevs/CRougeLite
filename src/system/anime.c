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

SpriteAnimation createSpriteAnimation(int numFrames, char **frameNames,
                                      int framesPerSecond, int loop) {
  SpriteAnimation anim = {
      .numOfFrames = numFrames,
      .framesPerSecond = framesPerSecond,
      .loop = loop,
      .currentFrame = 0,
      .finished = false,
  };

  anim.frameNames = malloc(sizeof(char *) * numFrames);

  for (int i = 0; i < numFrames; i++) {
    anim.frameNames[i] = malloc(strlen(frameNames[i]) + 1);
    strcpy(anim.frameNames[i], frameNames[i]);
  }

  return anim;
}

void drawSpriteAnimationPro(SpriteAnimation *anim, Rectangle dest,
                            Vector2 origin, float rotation, Color tint,
                            bool flipX) {
  int index = (int)(GetTime() * anim->framesPerSecond) % anim->numOfFrames;
  DrawAtlasSpritePro(anim->frameNames[index], dest, origin, rotation, tint,
                     flipX);
}

void disposeSpriteAnimation(SpriteAnimation *anim) {
  for (int i = 0; i < anim->numOfFrames; i++) {
    free(anim->frameNames[i]);
  }

  free(anim->frameNames);
}
