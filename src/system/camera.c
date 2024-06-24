/*********************************************************
 *
 *
 *     ██████╗ █████╗ ███╗   ███╗███████╗██████╗  █████╗
 *    ██╔════╝██╔══██╗████╗ ████║██╔════╝██╔══██╗██╔══██╗
 *    ██║     ███████║██╔████╔██║█████╗  ██████╔╝███████║
 *    ██║     ██╔══██║██║╚██╔╝██║██╔══╝  ██╔══██╗██╔══██║
 *    ╚██████╗██║  ██║██║ ╚═╝ ██║███████╗██║  ██║██║  ██║
 *     ╚═════╝╚═╝  ╚═╝╚═╝     ╚═╝╚══════╝╚═╝  ╚═╝╚═╝  ╚═╝
 *
 *
 *    Camera module
 *
 *
 *********************************************************/

#include "camera.h"
#include <raylib.h>

void initCamera() {
  gameState->camera = (Camera2D){
      .target = (Vector2){0, 0},
      .zoom = gameState->settings.zoom,
      .offset = (Vector2){SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f},
      .rotation = 0.0f,
  };
}

void updateCamera() {
  gameState->camera.target = gameState->players[0].object.transform.position;
  gameState->camera.offset =
      (Vector2){GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
  gameState->camera.zoom = gameState->settings.zoom;
}
