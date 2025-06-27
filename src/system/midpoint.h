/***************************************************************
 *
 *    ███╗   ███╗██╗██████╗ ██████╗  ██████╗ ██╗███╗   ██╗████████╗
 *    ████╗ ████║██║██╔══██╗██╔══██╗██╔═══██╗██║████╗  ██║╚══██╔══╝
 *    ██╔████╔██║██║██║  ██║██████╔╝██║   ██║██║██╔██╗ ██║   ██║   
 *    ██║╚██╔╝██║██║██║  ██║██╔═══╝ ██║   ██║██║██║╚██╗██║   ██║   
 *    ██║ ╚═╝ ██║██║██████╔╝██║     ╚██████╔╝██║██║ ╚████║   ██║   
 *    ╚═╝     ╚═╝╚═╝╚═════╝ ╚═╝      ╚═════╝ ╚═╝╚═╝  ╚═══╝   ╚═╝   
 *                                                                    
 *    Midpoint Line Algorithm Implementation
 *    Used for line-of-sight calculations and ray-casting.
 *
 **************************************************************/

#ifndef MIDPOINT_H
#define MIDPOINT_H

#include <stdbool.h>
#include <raylib.h>

/**
 * midpoint - Implements midpoint line algorithm to check if a path is walkable
 *
 * @param p1 Starting point coordinates
 * @param p2 Ending point coordinates
 * @param swapCoord Whether to swap x and y coordinates (for steep lines)
 * @return true if the entire line is walkable, false otherwise
 */
bool midpoint(Vector2 p1, Vector2 p2, bool swapCoord);

/**
 * lineOfSight - Determines if there's a clear line of sight between two points
 * 
 * @param p1 Starting point coordinates
 * @param p2 Ending point coordinates
 * @return true if there's a clear line of sight, false otherwise
 */
bool lineOfSight(Vector2 p1, Vector2 p2);

// Helper function declaration (if sign is not defined elsewhere)
int sign(int value);

#endif // MIDPOINT_H