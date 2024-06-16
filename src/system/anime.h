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

#ifndef ANIME_H
#define ANIME_H

#include "../CRougeLite.h"
SpriteAnimation createSpriteAnimation(int numFrames, char **frameNames,
                                      int framesPerSecond, int loop);

void drawSpriteAnimationPro(SpriteAnimation *anim, Rectangle dest,
                            Vector2 origin, float rotation, Color tint,
                            bool flipX);


void disposeSpriteAnimation(SpriteAnimation *anim);

#endif // ANIME_H
