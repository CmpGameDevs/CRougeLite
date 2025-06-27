/***************************************************************
 *
 *    ███╗   ███╗██╗███╗   ██╗██╗  ██╗███████╗ █████╗ ██████╗
 *    ████╗ ████║██║████╗  ██║██║  ██║██╔════╝██╔══██╗██╔══██╗
 *    ██╔████╔██║██║██╔██╗ ██║███████║█████╗  ███████║██████╔╝
 *    ██║╚██╔╝██║██║██║╚██╗██║██╔══██║██╔══╝  ██╔══██║██╔═══╝
 *    ██║ ╚═╝ ██║██║██║ ╚████║██║  ██║███████╗██║  ██║██║
 *    ╚═╝     ╚═╝╚═╝╚═╝  ╚═══╝╚═╝  ╚═╝╚══════╝╚═╝  ╚═╝╚═╝
 *
 *    Min Heap implementation for priority-based operations.
 *    Used primarily for pathfinding algorithms.
 *
 **************************************************************/

#ifndef MINHEAP_H
#define MINHEAP_H

#include "../structs.h"

/**
 * initHeap - Initialize a new min heap with specified capacity
 * 
 * @param h Pointer to the MinHeap structure
 * @param capacity Initial capacity of the heap
 */
void initHeap(MinHeap* h, int capacity);

/**
 * resizeHeap - Increase the capacity of the heap
 * 
 * @param h Pointer to the MinHeap structure
 */
void resizeHeap(MinHeap* h);

/**
 * push - Add a new element to the heap
 * 
 * @param h Pointer to the MinHeap structure
 * @param value The pPair value to add to the heap
 */
void push(MinHeap* h, pPair value);

/**
 * pop - Remove and return the minimum element from the heap
 * 
 * @param h Pointer to the MinHeap structure
 * @return The minimum pPair value from the heap
 */
pPair pop(MinHeap* h);

/**
 * isEmpty - Check if the heap is empty
 * 
 * @param h Pointer to the MinHeap structure
 * @return Non-zero if the heap is empty, zero otherwise
 */
int isEmpty(MinHeap* h);

/**
 * freeHeap - Release all resources used by the heap
 * 
 * @param h Pointer to the MinHeap structure
 */
void freeHeap(MinHeap* h);

#endif // MINHEAP_H