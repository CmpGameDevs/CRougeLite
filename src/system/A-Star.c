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

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <stdbool.h>
#include <string.h>
#include "minheap.h"
#include "A-Star.h"
#include "map.h"
#include "../structs.h"

// Direction arrays for 8-directional movement
int dx[] = {-1, 0, 1, 0, -1, -1, 1, 1}; // North, West, South, East, NW, NE, SW, SE
int dy[] = {0, -1, 0, 1, -1, 1, -1, 1};

/**
 * isValid - Checks whether given cell coordinates are valid
 *
 * @param row Row index of the cell
 * @param col Column index of the cell
 * @param rows Total rows in the grid
 * @param cols Total columns in the grid
 * @return Non-zero if valid, zero otherwise
 */
int isValid(int row, int col, int rows, int cols)
{
    return (row >= 0) && (row < rows) && (col >= 0) && (col < cols);
}

/**
 * isDestination - Checks whether the given cell is the destination
 *
 * @param row Row index of the cell to check
 * @param col Column index of the cell to check
 * @param dest Coordinates of the destination
 * @return 1 if destination, 0 otherwise
 */
int isDestination(int row, int col, CoordPair dest)
{
    if (row == dest.first && col == dest.second)
        return 1;
    else
        return 0;
}

/**
 * calculateHValue - Calculate the heuristic value (Euclidean distance)
 *
 * @param row Row index of the cell
 * @param col Column index of the cell
 * @param dest Coordinates of the destination
 * @return The calculated heuristic value
 */
double calculateHValue(int row, int col, CoordPair dest)
{
    return sqrt((row - dest.first) * (row - dest.first) +
                (col - dest.second) * (col - dest.second));
}

/**
 * tracePath - Reconstructs and prints the found path
 *
 * @param cellDetails The grid containing cell details
 * @param dest Coordinates of the destination
 * @param rows Total rows in the grid
 * @param cols Total columns in the grid
 */
CoordPair *tracePath(cell **cellDetails, CoordPair dest, int rows, int cols, int *pathLength)
{
    int row = dest.first;
    int col = dest.second;

    // Stack to store the path
    CoordPair *Path = (CoordPair *)malloc(rows * cols * sizeof(CoordPair));
    int top = -1;

    // Trace the path back from destination to source
    while (!(cellDetails[row][col].parent_i == row &&
             cellDetails[row][col].parent_j == col))
    {
        Path[++top].first = row;
        Path[top].second = col;
        int temp_row = cellDetails[row][col].parent_i;
        int temp_col = cellDetails[row][col].parent_j;
        row = temp_row;
        col = temp_col;
    }

    // Add source to path
    Path[++top].first = row;
    Path[top].second = col;

    *pathLength = top + 1;
    CoordPair *result = (CoordPair *)malloc((*pathLength) * sizeof(CoordPair));
    int index = 0;

    for (int i = 0; i <= top; i++)
    {
        result[i] = Path[top - i];
    }

    free(Path);
    return result;
}

/**
 * aStarSearch - Performs A* search algorithm on a grid
 *
 * @param grid The grid representation (1 for walkable, 0 for blocked)
 * @param rows Total rows in the grid
 * @param cols Total columns in the grid
 * @param src Source coordinates
 * @param dest Destination coordinates
 */
CoordPair *aStarSearch(CoordPair src, CoordPair dest, int *pathLength)
{
    GameState *game_system = gameState;
    Map *map = &(game_system->map);
    int rows = map->numOfRows;
    int cols = map->numOfCols;
    *pathLength = 0;
    // Validate source and destination
    if (isValid(src.first, src.second, rows, cols) == 0)
    {
        return NULL;
    }

    if (isValid(dest.first, dest.second, rows, cols) == 0)
    {
        return NULL;
    }

    if (isWalkable(src.first, src.second) == false || isWalkable(dest.first, dest.second) == false)
    {
        return NULL;
    }

    if (isDestination(src.first, src.second, dest) == 1)
    {
        return NULL;
    }

    // Dynamically allocate closedList
    int **closedList = (int **)malloc(rows * sizeof(int *));
    for (int i = 0; i < rows; i++)
    {
        closedList[i] = (int *)calloc(cols, sizeof(int));
    }

    // Dynamically allocate cellDetails
    cell **cellDetails = (cell **)malloc(rows * sizeof(cell *));
    for (int i = 0; i < rows; i++)
    {
        cellDetails[i] = (cell *)malloc(cols * sizeof(cell));
        for (int j = 0; j < cols; j++)
        {
            cellDetails[i][j].f = FLT_MAX;
            cellDetails[i][j].g = FLT_MAX;
            cellDetails[i][j].h = FLT_MAX;
            cellDetails[i][j].parent_i = -1;
            cellDetails[i][j].parent_j = -1;
        }
    }

    // Initialize start cell
    int i = src.first;
    int j = src.second;
    cellDetails[i][j].f = 0.0;
    cellDetails[i][j].g = 0.0;
    cellDetails[i][j].h = 0.0;
    cellDetails[i][j].parent_i = i;
    cellDetails[i][j].parent_j = j;

    // Initialize MinHeap for open list
    MinHeap openList;
    initHeap(&openList, rows * cols);

    // Add start node to open list
    pPair startNode;
    startNode.first = 0.0;
    startNode.second.first = i;
    startNode.second.second = j;
    push(&openList, startNode);

    while (!isEmpty(&openList))
    {
        // Get the cell with minimum f value
        pPair p = pop(&openList);

        i = p.second.first;
        j = p.second.second;

        closedList[i][j] = 1;

        double gNew, hNew, fNew;
        for (int x = 0; x < 4; x++)
        {
            int newRow = i + dx[x];
            int newCol = j + dy[x];

            if (isValid(newRow, newCol, rows, cols))
            {
                if (isDestination(newRow, newCol, dest) == 1)
                {
                    cellDetails[newRow][newCol].parent_i = i;
                    cellDetails[newRow][newCol].parent_j = j;
                    CoordPair* path = tracePath(cellDetails, dest, rows, cols, pathLength);

                    // Clean up resources
                    for (int i = 0; i < rows; i++)
                    {
                        free(closedList[i]);
                        free(cellDetails[i]);
                    }
                    free(closedList);
                    free(cellDetails);
                    freeHeap(&openList);

                    return path;
                }
                else if (closedList[newRow][newCol] == 0 &&
                         isWalkable(newRow, newCol) == true)
                {
                    gNew = cellDetails[i][j].g + 1.0;
                    hNew = calculateHValue(newRow, newCol, dest);
                    fNew = gNew + hNew;

                    if (cellDetails[newRow][newCol].f == FLT_MAX ||
                        cellDetails[newRow][newCol].f > fNew)
                    {
                        // Add to open list
                        pPair newNode;
                        newNode.first = fNew;
                        newNode.second.first = newRow;
                        newNode.second.second = newCol;
                        push(&openList, newNode);

                        // Update cell details
                        cellDetails[newRow][newCol].f = fNew;
                        cellDetails[newRow][newCol].g = gNew;
                        cellDetails[newRow][newCol].h = hNew;
                        cellDetails[newRow][newCol].parent_i = i;
                        cellDetails[newRow][newCol].parent_j = j;
                    }
                }
            }
        }
    }

    // Clean up resources
    for (int i = 0; i < rows; i++)
    {
        free(closedList[i]);
        free(cellDetails[i]);
    }
    free(closedList);
    free(cellDetails);
    freeHeap(&openList);

    return NULL; // No path found
}