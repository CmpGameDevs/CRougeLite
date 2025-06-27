#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <stdbool.h>
#include <string.h>
#include "../structs.h"
#include "map.h"
#include "midpoint.h"
#define swap(a, b) do { __typeof__(a) tmp = a; a = b; b = tmp; } while(0)

/**
 * midpoint - Implements midpoint line algorithm to check if a path is walkable
 *
 * @param p1 Starting point coordinates
 * @param p2 Ending point coordinates
 * @param swapCoord Whether to swap x and y coordinates (for steep lines)
 * @return true if the entire line is walkable, false otherwise
 */
bool midpoint(Vector2 p1, Vector2 p2, bool swapCoord)
{
    if (swapCoord)
    {
        swap(p1.x, p1.y);
        swap(p2.x, p2.y);
    }
    GameState *game_system = gameState;
    Map *map = &(game_system->map);
    if (p1.x > p2.x)
        swap(p1, p2);
    int dx = p2.x - p1.x;
    int dy = p2.y - p1.y;
    int change = dy < 0 ? -1 : 1;
    dy *= change;
    int f = dx - 2 * dy;
    int y = p1.y;
    for (int x = p1.x; x <= p2.x; x++)
    {
        if (swapCoord)
        {
            if (!isWalkable(y, x))
            {
                return false;
            }
        }
        else
        {
            if (!isWalkable(x, y))
            {
                return false;
            }
        }
        if (f < 0)
        {
            y += change;
            f += 2 * dx - 2 * dy;
        }
        else
        {
            f += -2 * dy;
        }
    }
    return true; 
}

/**
 * lineOfSight - Determines if there's a clear line of sight between two points
 * 
 * @param p1 Starting point coordinates
 * @param p2 Ending point coordinates
 * @return true if there's a clear line of sight, false otherwise
 */
bool lineOfSight(Vector2 p1, Vector2 p2)
{
    if (fabs(p1.x - p2.x) <= fabs(p1.y - p2.y))
    {
        return midpoint(p1, p2, true);
    }
    return midpoint(p1, p2, false);
}