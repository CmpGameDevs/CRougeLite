#include "input.h"
#include <raylib.h>

void handleInput() {
  if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_Q)) {
    CloseWindow();
  }
}
