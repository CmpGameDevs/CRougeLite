#ifndef PATH_H
#define PATH_H

#include "../CRougeLite.h"
#include <math.h>

float functionValue(int pathCode, float x);
float path(float x, float amp, float freq, Vector2 dest);
Vector2 RotatePoint(Vector2 point,Vector2 center, float theta);



#endif