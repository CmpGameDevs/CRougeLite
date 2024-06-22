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

// API

/*
 * getSrcRect
 *
 * Get the source rectangle of the frame in the atlas.
 * frameName: the name of the frame in the atlas.
 * Returns: the source rectangle of the frame.
*/
Rectangle getSrcRect(char *frameName);

void drawSpriteAnimationPro(SpriteAnimation *anim, Rectangle dest,
                            Vector2 origin, float rotation, Color tint,
                            bool flipX);



#endif // ANIME_H
// vim: fdm=marker
