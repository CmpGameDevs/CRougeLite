/***************************************************************
 *
 *
 *
 *   ██████╗ ███████╗██████╗ ██╗   ██╗ ██████╗
 *   ██╔══██╗██╔════╝██╔══██╗██║   ██║██╔════╝
 *   ██║  ██║█████╗  ██████╔╝██║   ██║██║  ███╗
 *   ██║  ██║██╔══╝  ██╔══██╗██║   ██║██║   ██║
 *   ██████╔╝███████╗██████╔╝╚██████╔╝╚██████╔╝
 *   ╚═════╝ ╚══════╝╚═════╝  ╚═════╝  ╚═════╝
 *
 *
 *   Debug Menu
 *
 **************************************************************/

#include "debugMenu.h"

#define RAYGUI_IMPLEMENTATION
#include "../libs/raygui.h"

static const int CHECKBOX_SIZE = 24;
static const int CHECKBOX_PADDING = 20;
static const int CHECKBOX_FONT_SIZE = 24;

void drawDebugMenu() {
  GuiSetStyle(DEFAULT, TEXT_SIZE, CHECKBOX_FONT_SIZE);
  GuiSetStyle(DEFAULT, CHECK_PADDING, CHECKBOX_PADDING);

  GuiCheckBox(
      (Rectangle){GetScreenWidth() - 200, 60, CHECKBOX_SIZE, CHECKBOX_SIZE},
      "Show Textures", &gameState->settings.showTextures);
  GuiCheckBox(
      (Rectangle){GetScreenWidth() - 200, 90, CHECKBOX_SIZE, CHECKBOX_SIZE},
      "Show Hitboxes", &gameState->settings.showColliders);
  GuiCheckBox(
      (Rectangle){GetScreenWidth() - 200, 120, CHECKBOX_SIZE, CHECKBOX_SIZE},
      "Show FPS", &gameState->settings.showFPS);
  GuiCheckBox(
      (Rectangle){GetScreenWidth() - 200, 150, CHECKBOX_SIZE, CHECKBOX_SIZE},
      "Play Music", &gameState->settings.playMusic);
}
