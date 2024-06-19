#include "path.h"

float scaleParameter = 25;
float frequency = 150;
float a = -.2;
float b = .5;
float functionValue(int pathCode, float x)
{
    float calc = scaleParameter * sinf(x / frequency);
    if (pathCode == 0)
    {
        return 0;
    }
    if (pathCode == 1)
    {
        return calc;
    }
    if (pathCode == 2)
    {
        return -(scaleParameter / 2) * sinf(x / frequency);
    }
    if (pathCode == 3)
    {
        return calc + scaleParameter * sinf((x / frequency) + a * scaleParameter) ;
    }
    if (pathCode == 4)
    {
        return calc + scaleParameter * sinf((x / frequency) + a * frequency) + scaleParameter * b * sinf(2 * x / frequency);
    }
}

Vector2 RotatePoint(Vector2 point,Vector2 center, float theta)
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
        rotatedY + center.y
    };

    return newPoint;
}