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

  if (gameState->settings.showInventory)
    drawInventory();

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
