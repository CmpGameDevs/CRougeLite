#include "draw.h"

#include "../game/combatAction.h"
#include "../game/debugMenu.h"
#include "../game/enemy.h"
#include "../game/player.h"
#include "atlas.h"
#include "map.h"
#include <raylib.h>

// ***************************
// Private Function Prototypes
// ***************************
static void drawBackgroundImage(Texture2D texture);
static void drawInventory();
static void drawInteractionUI();

/**
 * drawAtlasSpritePro - draws a sprite from the atlas with the given parameters.
 * 
 * @param filename The name of the sprite file in the atlas.
 * @param dest The destination rectangle where the sprite will be drawn.
 * @param origin The origin point for rotation.
 * @param rotation The rotation angle in radians.
 * @param tint The color tint to apply to the sprite.
 * @param flipX Whether to flip the sprite horizontally.
 */
void drawAtlasSpritePro(char *filename, Rectangle dest, Vector2 origin,
                        float rotation, Color tint, bool flipX) {
  AtlasImage image = getAtlasImage(filename);

  if (image.filename != NULL) {
    if (flipX) {
      image.source.width *= -1;
    }

    if (gameState->settings.showTextures)
      DrawTexturePro(gameState->atlasTexture, image.source, dest, origin,
                     rotation, tint);
    if (gameState->settings.showTexturesBounds)
      DrawRectangleLines(dest.x, dest.y, dest.width, dest.height,
                         GetColor(0x8B4000AA));
  }
}

/**
 * drawColliders - draws the colliders of the map, players, enemies, and combat actions.
 */
void drawColliders() {
  // draw map colliders
  Map *map = &(gameState->map);
  for (int i = 0; i < map->numOfRows; i++) {
    for (int j = 0; j < map->numOfCols; j++) {
      Rectangle bounds = {
            .x = j * map->tileWidth * map->scale,
            .y = i * map->tileHeight * map->scale,
            .width = map->tileWidth * map->scale,
            .height = map->tileHeight * map->scale,
        };
      Color color = {0,0,0,0};
      if (map->mapIds[i][j][LAYER_WALLS] != -1) {
        color = GREEN;
      }
      if (map->mapIds[i][j][LAYER_INTERACTABLE] != -1) {
        color = YELLOW;
      }
      if (map->mapIds[i][j][LAYER_PICKABLE] != -1) {
        color = PURPLE;
      }
      
      DrawRectangleLines(bounds.x,
                         bounds.y,
                         bounds.width,
                         bounds.height,
                         color);
    }
  }

  // draw players colliders
  Player *players = gameState->players;
  for (int i = 0; i < gameState->numOfPlayers; i++) {
    Rectangle bounds = players[i].object.collider.bounds;
    Color color = (players[i].object.collider.isColliding ? BLUE : RED);
    DrawRectangleLines(bounds.x,
                       bounds.y,
                       bounds.width,
                       bounds.height, color);
  }

  // draw enemies colliders
  Enemy *enemies = gameState->enemies;
  for (int i = 0; i < gameState->numOfEnemies; i++) {
    Rectangle bounds = enemies[i].object.collider.bounds;
    Color color = (enemies[i].object.collider.isColliding ? BLUE : RED);
    DrawRectangleLines(bounds.x,
                       bounds.y,
                       bounds.width,
                       bounds.height, color);
  }

  // draw bullet actions colliders
  CombatAction *actions = gameState->combatActions;
  for (int i = 0; i < gameState->numOfCombatActions; i++) {
    GameObject *object = NULL;
    switch (actions[i].type) {
      case ACTION_BULLET:
        object = &(actions[i].action.bullet.bulletInfo.object);
        break;
      case ACTION_SLASH:
        object = &(actions[i].action.slash.slashInfo.object);
        break;
      default:
        break;
    }
    Rectangle bounds = object->collider.bounds;
    Color color = (object->collider.isColliding ? BLUE : RED);
    DrawRectangleLines(bounds.x,
                       bounds.y,
                       bounds.width,
                       bounds.height, color);
  }
}

/**
 * drawScene - draws the entire game scene including map, players, enemies,
 */
void drawScene() {
  BeginDrawing();

  ClearBackground(GetColor(0x25131aff));

  if (!gameState->isGameStarted) {
    drawBackgroundImage(gameState->mainMenuBackground);
    
    if (gameState->settings.showDebugMenu) {
      drawDebugMenu();
    }
    EndDrawing();
    return;
  }

  BeginMode2D(gameState->camera);
  drawMap();

  drawPlayers();
  drawEnemies();

  drawCombatActions();

  if (gameState->settings.showPaths) {
    drawEnemyPaths();
  }

  Vector2 cur = GetMousePosition();
  Vector2 worldPos = GetScreenToWorld2D(cur, gameState->camera);
  DrawCircle(worldPos.x, worldPos.y, 5, RED);

  if (gameState->settings.showColliders)
    drawColliders();
  EndMode2D();
  if (gameState->settings.showDebugMenu)
    drawDebugMenu();

  drawInteractionUI();

  if (gameState->settings.showUI)
  {
    drawInventory();
    drawWeaponSelectionUI();
  }

  if (gameState->settings.showFPS)
    DrawFPS(10, 10);

  EndDrawing();
}

/**
 * drawHealthBar - draws a health bar above an entity if damaged recently
 * 
 * @param position The world position to draw the health bar above
 * @param currentHealth The current health of the entity
 * @param maxHealth The maximum health of the entity
 * @param lastUpdateTime The time when the entity last took damage
 */
void drawHealthBar(Vector2 position, float currentHealth, float maxHealth, float lastUpdateTime) {
  double currentTime = GetTime();
  double timeSinceLastDamage = currentTime - lastUpdateTime;
  
  // Only show health bar if damaged within last 3 seconds
  if (timeSinceLastDamage > 3.0 || maxHealth == currentHealth || maxHealth <= 0) {
    return;
  }
  
  const int barWidth = 48;
  const int barHeight = 8;
  const int barOffsetY = -12.5;
  const int segmentWidth = 4;
  const int segmentSpacing = 1;

  float healthPercentage = currentHealth / maxHealth;
  healthPercentage = fmax(0.0f, fmin(1.0f, healthPercentage));
  
  Vector2 barPosition = {
    position.x - barWidth / 2,
    position.y + barOffsetY
  };
  
  Rectangle backgroundPanel = {
    barPosition.x - 3,
    barPosition.y - 3,
    barWidth + 6,
    barHeight + 6
  };
  DrawRectangleRec(backgroundPanel, (Color){0, 0, 0, 160});
  DrawRectangleLines(backgroundPanel.x, backgroundPanel.y, backgroundPanel.width, backgroundPanel.height, (Color){100, 100, 100, 200});
  
  // NOTE: This draws the health bar into segments for a blocky feel
  int totalSegments = (barWidth - segmentSpacing) / (segmentWidth + segmentSpacing);
  int filledSegments = (int)ceil(totalSegments * healthPercentage);
  
  for (int i = 0; i < totalSegments; i++) {
    int segmentX = barPosition.x + i * (segmentWidth + segmentSpacing);
    int segmentY = barPosition.y + 1;
    
    Rectangle segment = {
      segmentX,
      segmentY,
      segmentWidth,
      barHeight - 2
    };
    
    Color segmentColor;
    if (i < filledSegments) {
      if (healthPercentage > 0.6f) {
        segmentColor = (Color){60, 180, 60, 255}; // Green
      } else if (healthPercentage > 0.3f) {
        segmentColor = (Color){220, 180, 40, 255}; // Yellow
      } else {
        segmentColor = (Color){200, 60, 60, 255}; // Red
      }
    } else {
      segmentColor = (Color){40, 40, 40, 200};
    }

    DrawRectangleRec(segment, segmentColor);
    
    if (i < filledSegments) {
      DrawRectangleLines(segment.x, segment.y, segment.width, segment.height, (Color){255, 255, 255, 100});
    }
  }
}

// *****************
// PRIVATE FUNCTIONS
// *****************

/**
 * drawBackgroundImage - draws a background image for the screen
 */
static void drawBackgroundImage(Texture2D texture) {
  if (texture.id == 0) return;

  int screenWidth = GetScreenWidth();
  int screenHeight = GetScreenHeight();
  
  Rectangle source = {0, 0, (float)texture.width, (float)texture.height};
  Rectangle dest = {0, 0, (float)screenWidth, (float)screenHeight};
  
  DrawTexturePro(texture, source, dest, (Vector2){0, 0}, 0.0f, WHITE);
}

/**
 * drawInventory - draws the player's inventory at the bottom of the screen.
 */
static void drawInventory() {
  int selected_player = 0; // TODO: Make this configurable for multiplayer
  Player *player = &(gameState->players[selected_player]);
  
  int screenWidth = GetScreenWidth();
  int screenHeight = GetScreenHeight();
  
  const int slotSize = 48;
  const int slotSpacing = 4;
  const int totalSlots = MAX_COLLECTED_ITEMS;
  const int totalWidth = (slotSize * totalSlots) + (slotSpacing * (totalSlots - 1));
  
  int startX = (screenWidth - totalWidth) / 2;
  int startY = screenHeight - slotSize - 15;
    Rectangle panelBounds = {
    .x = startX - 6,
    .y = startY - 6,
    .width = totalWidth + 12,
    .height = slotSize + 12
  };
  DrawRectangleRec(panelBounds, (Color){0, 0, 0, 64});
  DrawRectangleLines(panelBounds.x, panelBounds.y, panelBounds.width, panelBounds.height, (Color){120, 120, 120, 144});
  
  // Draw inventory slots
  for (int i = 0; i < MAX_COLLECTED_ITEMS; i++) {
    int slotX = startX + (i * (slotSize + slotSpacing));
    int slotY = startY;
    
    Rectangle slotRect = {
      .x = slotX,
      .y = slotY,
      .width = slotSize,
      .height = slotSize    };
    DrawRectangleRec(slotRect, (Color){40, 40, 40, 200});
    DrawRectangleLines(slotRect.x, slotRect.y, slotRect.width, slotRect.height, (Color){100, 100, 100, 200});
    
    CollectibleItem *item = &(player->collectedItems[i]);
    
    if (item->tileId != -1 && item->count > 0) {
      GameState *game_system = gameState;
      Map *map = &(game_system->map);
      TilesMapper *tiles_mapper = &(map->tilesMapper);
      
      if (item->tileId < MAX_TILES_NUM && tiles_mapper->tileInfo[item->tileId].filename != NULL) {
        char buffer[256];
        char *tileName = tiles_mapper->tileInfo[item->tileId].filename;
        strncpy(buffer, tileName, sizeof(buffer) - 1);
        buffer[sizeof(buffer) - 1] = '\0';
        
        char *fileName = strtok(buffer, ".");

        Rectangle itemDest = {
          .x = slotX + 3,
          .y = slotY + 3,
          .width = slotSize - 6,
          .height = slotSize - 6
        };
        
        drawAtlasSpritePro(fileName, itemDest, (Vector2){0, 0}, 0.0f, WHITE, false);
        
        if (item->count > 1) {
          char countText[16];
          snprintf(countText, sizeof(countText), "%d", item->count);
          
          Vector2 countPos = {slotX + slotSize - 16, slotY + slotSize - 16};
          DrawText(countText, countPos.x + 1, countPos.y + 1, 12, BLACK); // Shadow
          DrawText(countText, countPos.x, countPos.y, 12, WHITE); // Text
        }
      }
    }
  }
}

/**
 * drawInteractionUI - draws the interaction UI for nearby interactable tiles.
 */
static void drawInteractionUI() {
  int selected_player = 0; // TODO: Make this configurable for multiplayer
  Player *player = &(gameState->players[selected_player]);
  
  if (player->interactableTileIndex == -1) return;
  
  Map *map = &(gameState->map);
  int row = player->interactableTileIndex / map->numOfCols;
  int col = player->interactableTileIndex % map->numOfCols;
  int tileId = map->mapIds[row][col][LAYER_INTERACTABLE];
  
  if (tileId == -1) return;
  
  float tileWorldX = col * map->tileWidth * map->scale + (map->tileWidth * map->scale) / 2;
  float tileWorldY = row * map->tileHeight * map->scale + (map->tileHeight * map->scale) / 2;
  Vector2 tileScreenPos = GetWorldToScreen2D((Vector2){tileWorldX, tileWorldY}, gameState->camera);
  const char* promptText = "F";
  int fontSize = 24;
  const int promptSize = 40;
  
  Vector2 promptPos = {
    tileScreenPos.x - promptSize / 2,
    tileScreenPos.y - 80
  };
  
  Rectangle promptBg = {
    promptPos.x,
    promptPos.y,
    promptSize,
    promptSize
  };
  DrawRectangleRec(promptBg, (Color){40, 40, 40, 200});
  DrawRectangleLines(promptBg.x, promptBg.y, promptBg.width, promptBg.height, (Color){100, 100, 100, 200});
  
  Vector2 textSize = MeasureTextEx(GetFontDefault(), promptText, fontSize, 2);
  Vector2 textPos = {
    promptPos.x + (promptSize - textSize.x) / 2,
    promptPos.y + (promptSize - textSize.y) / 2
  };
  
  DrawText(promptText, textPos.x + 1, textPos.y + 1, fontSize, BLACK); // Shadow
  DrawText(promptText, textPos.x, textPos.y, fontSize, WHITE); // Text
  
  InteractableMapping *mapping = getInteractableMapping(tileId);
  if (mapping && mapping->requiredItem != -1) {
    TilesMapper *tiles_mapper = &(map->tilesMapper);
    if (mapping->requiredItem < MAX_TILES_NUM && tiles_mapper->tileInfo[mapping->requiredItem].filename != NULL) {
      char buffer[256];
      char *tileName = tiles_mapper->tileInfo[mapping->requiredItem].filename;
      strncpy(buffer, tileName, sizeof(buffer) - 1);
      buffer[sizeof(buffer) - 1] = '\0';
      
      char *fileName = strtok(buffer, ".");
      const int itemIconSize = 28;
      Rectangle itemDest = {
        promptPos.x + promptSize + 10,
        promptPos.y + (promptSize - itemIconSize) / 2,
        itemIconSize,
        itemIconSize
      };
      
      Rectangle itemBg = {
        itemDest.x - 3,
        itemDest.y - 3,
        itemDest.width + 6,
        itemDest.height + 6
      };
      DrawRectangleRec(itemBg, (Color){40, 40, 40, 200});
      DrawRectangleLines(itemBg.x, itemBg.y, itemBg.width, itemBg.height, (Color){100, 100, 100, 200});
      
      drawAtlasSpritePro(fileName, itemDest, (Vector2){0, 0}, 0.0f, WHITE, false);
    }
  }
}

/**
 * drawWeaponSelectionUI - draws the weapon selection UI showing current, previous, and next weapons with cooldown indicator
 */
void drawWeaponSelectionUI() {
  int selected_player = 0;
  Player *player = &(gameState->players[selected_player]);
  WeaponsInventory *inventory = &(player->inventory);
  
  if (inventory->currentNumOfWeapons <= 0) return;
  
  int screenWidth = GetScreenWidth();
  int screenHeight = GetScreenHeight();
  
  const int sideWeaponSlotSize = 40;
  const int currentWeaponSlotSize = 56;
  const int weaponSpacing = 6;
  const int totalSlots = 3;
  const int totalHeight = (sideWeaponSlotSize * 2) + currentWeaponSlotSize + (weaponSpacing * (totalSlots - 1));
  
  int startX = screenWidth - currentWeaponSlotSize - 20;
  int startY = screenHeight - totalHeight - 20;
  
  // Background panel (adjust for vertical layout and larger current slot)
  Rectangle panelBounds = {
    .x = startX - 6,
    .y = startY - 6,
    .width = currentWeaponSlotSize + 12,
    .height = totalHeight + 12
  };
  DrawRectangleRec(panelBounds, (Color){0, 0, 0, 160});
  DrawRectangleLines(panelBounds.x, panelBounds.y, panelBounds.width, panelBounds.height, (Color){100, 100, 100, 200});
  
  // Draw weapon slots (Previous, Current, Next) - vertical layout
  for (int slot = 0; slot < totalSlots; slot++) {
    // Calculate slot size and position based on whether it's current weapon
    bool isCurrentWeapon = (slot == 1);
    int slotSize = isCurrentWeapon ? currentWeaponSlotSize : sideWeaponSlotSize;
    
    // Calculate Y position for vertical stacking
    int slotY;
    if (slot == 0) { // Previous weapon (top)
      slotY = startY;
    } else if (slot == 1) { // Current weapon (middle)
      slotY = startY + sideWeaponSlotSize + weaponSpacing;
    } else { // slot == 2, Next weapon (bottom)
      slotY = startY + sideWeaponSlotSize + currentWeaponSlotSize + (weaponSpacing * 2);
    }
    
    // Center smaller slots horizontally with the larger current slot
    int slotX = isCurrentWeapon ? startX : startX + (currentWeaponSlotSize - sideWeaponSlotSize) / 2;
    
    Rectangle slotRect = {
      .x = slotX,
      .y = slotY,
      .width = slotSize,
      .height = slotSize
    };
    
    // Calculate weapon index for each slot
    int weaponIndex = -1;
    
    if (slot == 0) { // Previous weapon
      weaponIndex = inventory->currentWeapon - 1;
      if (weaponIndex < 0) weaponIndex = inventory->currentNumOfWeapons - 1;
    } else if (slot == 1) { // Current weapon
      weaponIndex = inventory->currentWeapon;
    } else if (slot == 2) { // Next weapon
      weaponIndex = inventory->currentWeapon + 1;
      if (weaponIndex >= inventory->currentNumOfWeapons) weaponIndex = 0;
    }
    
    // Draw slot background using weapon type colors
    Color slotBgColor, borderColor;
    if (weaponIndex >= 0 && weaponIndex < inventory->currentNumOfWeapons) {
      Weapon *weapon = &inventory->weapons[weaponIndex];
      
      if (weapon->type == RANGED_WEAPON) {
        // Blue theme for ranged weapons
        slotBgColor = isCurrentWeapon ? (Color){30, 60, 120, 200} : (Color){30, 60, 120, 120};
        borderColor = isCurrentWeapon ? (Color){70, 140, 255, 255} : (Color){70, 140, 255, 120};
      } else {
        // Orange/red theme for melee weapons
        slotBgColor = isCurrentWeapon ? (Color){120, 60, 30, 200} : (Color){120, 60, 30, 120};
        borderColor = isCurrentWeapon ? (Color){255, 140, 70, 255} : (Color){255, 140, 70, 120};
      }
    } else {
      // Default gray for empty slots
      slotBgColor = isCurrentWeapon ? (Color){40, 40, 40, 200} : (Color){40, 40, 40, 120};
      borderColor = isCurrentWeapon ? (Color){100, 100, 100, 255} : (Color){100, 100, 100, 120};
    }
    
    DrawRectangleRec(slotRect, slotBgColor);
    DrawRectangleLines(slotRect.x, slotRect.y, slotRect.width, slotRect.height, borderColor);
    
    // Draw weapon if valid index
    if (weaponIndex >= 0 && weaponIndex < inventory->currentNumOfWeapons) {
      Weapon *weapon = &inventory->weapons[weaponIndex];
      
      // Draw weapon name in the center of the slot
      const char *weaponName = weapon->name;
      int fontSize = isCurrentWeapon ? 8 : 6;
      int textWidth = MeasureText(weaponName, fontSize);
      
      // Word wrap for longer weapon names
      if (textWidth > slotSize - 8) {
        // Split weapon name into words
        char nameCopy[64];
        strncpy(nameCopy, weaponName, sizeof(nameCopy) - 1);
        nameCopy[sizeof(nameCopy) - 1] = '\0';
        
        char *firstWord = strtok(nameCopy, " ");
        char *secondWord = strtok(NULL, " ");
        
        if (firstWord && secondWord) {
          // Draw two lines
          int firstWidth = MeasureText(firstWord, fontSize);
          int secondWidth = MeasureText(secondWord, fontSize);
          
          Color textColor = isCurrentWeapon ? WHITE : (Color){255, 255, 255, 180};
          
          DrawText(firstWord, slotX + (slotSize - firstWidth) / 2, slotY + slotSize / 2 - fontSize - 2, fontSize, textColor);
          DrawText(secondWord, slotX + (slotSize - secondWidth) / 2, slotY + slotSize / 2 + 2, fontSize, textColor);
        } else {
          // Single word that's too long, draw it anyway
          Color textColor = isCurrentWeapon ? WHITE : (Color){255, 255, 255, 180};
          DrawText(weaponName, slotX + (slotSize - textWidth) / 2, slotY + slotSize / 2 - fontSize / 2, fontSize, textColor);
        }
      } else {
        // Single line text
        Color textColor = isCurrentWeapon ? WHITE : (Color){255, 255, 255, 180};
        DrawText(weaponName, slotX + (slotSize - textWidth) / 2, slotY + slotSize / 2 - fontSize / 2, fontSize, textColor);
      }
      
      // Draw cooldown indicator - shrinking overlay from top
      float cooldown = 0.0f;
      float remainingTime = 0.0f;
      
      if (weapon->type == RANGED_WEAPON) {
        cooldown = weapon->weapon.ranged.stats.cooldown;
        remainingTime = weapon->weapon.ranged.stats.lastUseTime;
      } else if (weapon->type == MELEE_WEAPON) {
        cooldown = weapon->weapon.melee.stats.cooldown;
        remainingTime = weapon->weapon.melee.stats.lastUseTime;
      }
      
      if (cooldown > 0.0f && remainingTime > 0.0f) {
        // Calculate progress: 1.0 = just used (full overlay), 0.0 = ready (no overlay)
        float cooldownPercent = remainingTime / cooldown;
        cooldownPercent = fmaxf(0.0f, fminf(1.0f, cooldownPercent)); // Clamp between 0 and 1
        
        // Shrinking overlay that covers from top, reducing in height as cooldown progresses
        int overlayHeight = (int)(slotSize * cooldownPercent);
        if (overlayHeight > 0) {
          Rectangle cooldownOverlay = {
            slotX,
            slotY,
            slotSize,
            overlayHeight
          };
          
          // Semi-transparent dark overlay with subtle red tint
          DrawRectangleRec(cooldownOverlay, (Color){20, 20, 20, 180});
          DrawRectangleRec(cooldownOverlay, (Color){60, 20, 20, 100});
        }
      }
      
      // Draw key binding for current weapon (small number in corner)
      if (isCurrentWeapon) {
        char keyText[8];
        snprintf(keyText, sizeof(keyText), "%d", weaponIndex + 1);
        
        // Small background circle for key binding
        int keySize = 14;
        Rectangle keyBg = {slotX + slotSize - keySize - 2, slotY + 2, keySize, keySize};
        DrawRectangleRec(keyBg, (Color){0, 0, 0, 200});
        DrawRectangleLines(keyBg.x, keyBg.y, keyBg.width, keyBg.height, (Color){255, 255, 100, 255});
        
        int keyTextWidth = MeasureText(keyText, 8);
        DrawText(keyText, keyBg.x + (keySize - keyTextWidth) / 2, keyBg.y + 3, 8, (Color){255, 255, 100, 255});
      }
    }
  }
  
  // Draw scroll wheel hint (positioned above the UI)
  const char *scrollHint = "Scroll";
  int hintWidth = MeasureText(scrollHint, 10);
  DrawText(scrollHint, startX + (currentWeaponSlotSize - hintWidth) / 2, startY - 18, 10, (Color){200, 200, 200, 180});
}
