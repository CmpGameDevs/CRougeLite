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
static const int RIGHT_MARGIN = 250;
static const int TOP_MARGIN = 60;

void drawDebugMenu() {
  GuiSetStyle(DEFAULT, TEXT_SIZE, CHECKBOX_FONT_SIZE);
  GuiSetStyle(DEFAULT, CHECK_PADDING, CHECKBOX_PADDING);

  // NOTE: this must match the len of the arrays
  const int numCheckboxes = 5;
  bool *checkboxes[] = {
      &gameState->settings.showTextures,
      &gameState->settings.showTexturesBounds,
      &gameState->settings.showColliders,
      &gameState->settings.showFPS,
      &gameState->settings.playMusic,
  };

  char *labels[] = {
      "Show Textures", "Show Texture Bounds", "Show Hitboxes",
      "Show FPS",      "Play Music",
  };

  const int numSliders = 1;
  float *sliders[] = {
      &gameState->settings.zoom,
  };
  char *sliderLabels[] = {
      "Zoom",
  };

  for (int i = 0; i < numCheckboxes; i++) {
    GuiCheckBox((Rectangle){GetScreenWidth() - RIGHT_MARGIN,
                            TOP_MARGIN + (CHECKBOX_SIZE + 10) * i,
                            CHECKBOX_SIZE, CHECKBOX_SIZE},
                labels[i], checkboxes[i]);
  }

  for (int i = 0; i < numSliders; i++) {
    GuiSlider(
        (Rectangle){GetScreenWidth() - RIGHT_MARGIN,
                    TOP_MARGIN + (CHECKBOX_SIZE + 10) * (numCheckboxes + i),
                    200, CHECKBOX_SIZE},
        sliderLabels[i], NULL, sliders[i], 0.0, 2.0);
  }
}
