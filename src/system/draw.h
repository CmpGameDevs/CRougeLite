#ifndef DRAW_H
#define DRAW_H

#include "../CRougeLite.h"

void drawScene();
void DrawAtlasSpritePro(char *filename, Rectangle dest, Vector2 origin,
                        float rotation, Color tint, bool flipX); 



#endif // DRAW_H
