#ifndef DRAW_H
#define DRAW_H

#include <raylib.h>
#include "../CRougeLite.h"

void drawColliders();
void drawScene();
void drawAtlasSpritePro(char *filename, Rectangle dest, Vector2 origin,
                        float rotation, Color tint, bool flipX);
void drawHealthBar(Vector2 position, float currentHealth, float maxHealth, float lastUpdateTime);
void drawWeaponSelectionUI();


#endif // DRAW_H
