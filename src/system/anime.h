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


/* getSrcRect
 *
 * Get the source rectangle for the current frame of the animation.
 *
 * @param animator - The animator to get the source rectangle from.
 *
 * @return - The source rectangle for the current frame of the animation.
 *
 */
Rectangle getSrcRect(Animator *animator);

#endif // ANIME_H
// vim: fdm=marker
