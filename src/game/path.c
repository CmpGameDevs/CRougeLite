#include "path.h"

float path(float x, float freq, float amp, Vector2 dest)
{
    float phase =  - dest.x * freq;
    return amp * sinf(x * freq + phase);
}

Vector2 RotatePoint(Vector2 point, Vector2 center, float theta)
{
    // Translate point to origin
    float translatedX = point.x - center.x;
    float translatedY = point.y - center.y;

    // Rotate point
    float rotatedX = translatedX * cosf(theta) - translatedY * sinf(theta);
    float rotatedY = translatedX * sinf(theta) + translatedY * cosf(theta);

    // Translate point back
    Vector2 newPoint = {
        rotatedX + center.x,
        rotatedY + center.y};

    return newPoint;
}