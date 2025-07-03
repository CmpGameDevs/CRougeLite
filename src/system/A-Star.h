/***************************************************************
 *
 *     █████╗   ███████╗████████╗ █████╗ ██████╗
 *    ██╔══██╗  ██╔════╝╚══██╔══╝██╔══██╗██╔══██╗
 *    ███████║  ███████╗   ██║   ███████║██████╔╝
 *    ██╔══██║  ╚════██║   ██║   ██╔══██║██╔══██╗
 *    ██║  ██║  ███████║   ██║   ██║  ██║██║  ██║
 *    ╚═╝  ╚═╝  ╚══════╝   ╚═╝   ╚═╝  ╚═╝╚═╝  ╚═╝
 *
 *    A* Pathfinding implementation for grid-based maps.
 *
 **************************************************************/

#ifndef A_STAR_H
#define A_STAR_H

#include "../structs.h"

/**
 * Cell structure to store details of a node in the grid
 */
typedef struct
{
    int parent_i, parent_j; // Parent cell coordinates
    double f, g, h;         // f = g + h (total cost, path cost, heuristic)
} cell;

/**
 * isValid - Checks whether given cell coordinates are valid
 *
 * @param row Row index of the cell
 * @param col Column index of the cell
 * @param rows Total rows in the grid
 * @param cols Total columns in the grid
 * @return Non-zero if valid, zero otherwise
 */
int isValid(int row, int col, int rows, int cols);

/**
 * isDestination - Checks whether the given cell is the destination
 *
 * @param row Row index of the cell to check
 * @param col Column index of the cell to check
 * @param dest Coordinates of the destination
 * @return 1 if destination, 0 otherwise
 */
int isDestination(int row, int col, CoordPair dest);

/**
 * calculateHValue - Calculate the heuristic value (Euclidean distance)
 *
 * @param row Row index of the cell
 * @param col Column index of the cell
 * @param dest Coordinates of the destination
 * @return The calculated heuristic value
 */
double calculateHValue(int row, int col, CoordPair dest);

/**
 * tracePath - Reconstructs and prints the found path
 *
 * @param cellDetails The grid containing cell details
 * @param dest Coordinates of the destination
 * @param rows Total rows in the grid
 * @param cols Total columns in the grid
 * @param pathLength Pointer to store the length of the path found
 */
CoordPair *tracePath(cell **cellDetails, CoordPair dest, int rows, int cols, int *pathLength);

/**
 * aStarSearch - Performs A* search algorithm on a grid
 *
 * @param src Source coordinates
 * @param dest Destination coordinates
 * @param pathLength Pointer to store the length of the path found
 */
CoordPair *aStarSearch(CoordPair src, CoordPair dest, int *pathLength);

#endif // A_STAR_H