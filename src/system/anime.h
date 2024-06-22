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

/* setState
 *
 * Set the state of the animator to the specified state.
 *
 * @param animator - The animator to set the state of.
 * @param state - The state to set the animator to.
 *
 */
void setState(Animator *animator, int state);

/* updateAnimator
 *
 * Update the animator to the next frame of the animation.
 *
 * @param animator - The animator to update.
 *
 */
void updateAnimator(Animator *animator);

/* drawAnimator
 *
 * Draw the animator to the screen.
 *
 * @param animator - The animator to draw.
 * @param transform - The transform of the animator.
 * @param tint - The tint color of the animator.
 * @param flipX - Whether to flip the animator on the x-axis.
 *
 */
void drawAnimator(Animator *animator, CTransform *transform, Color tint,
                  bool flipX);

#endif // ANIME_H
// vim: fdm=marker
