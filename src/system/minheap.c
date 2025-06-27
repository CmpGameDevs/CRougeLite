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
 *    Used primarily for A* pathfinding algorithms.
 *
 **************************************************************/

#include "../structs.h"
#include <stdlib.h>
#include <stdio.h>

// ***************************
// Local Function Definitions
// ***************************

/**
 * swap - Helper function to exchange two pPair values
 * 
 * @param a Pointer to first pPair
 * @param b Pointer to second pPair
 */
void swap(pPair* a, pPair* b) {
    pPair temp = *a;
    *a = *b;
    *b = temp;
}

// ***************************
// Public Function Definitions
// ***************************

/**
 * initHeap - Initialize a new min heap with specified capacity
 * 
 * @param h Pointer to the MinHeap structure
 * @param capacity Initial capacity of the heap
 */
void initHeap(MinHeap* h, int capacity) {
    h->data = (pPair*)malloc(capacity * sizeof(pPair));
    h->size = 0;
    h->capacity = capacity;
}

/**
 * resizeHeap - Double the capacity of the heap when needed
 * 
 * @param h Pointer to the MinHeap structure
 */
void resizeHeap(MinHeap* h) {
    h->capacity *= 2;
    h->data = (pPair*)realloc(h->data, h->capacity * sizeof(pPair));
}

/**
 * push - Add a new element to the heap and maintain heap property
 * Time complexity: O(log n)
 * 
 * @param h Pointer to the MinHeap structure
 * @param value The pPair value to add to the heap
 */
void push(MinHeap* h, pPair value) {
    // Resize if needed
    if (h->size == h->capacity) {
        resizeHeap(h);
    }

    // Insert at the end
    int i = h->size++;
    h->data[i] = value;

    // Heapify up: maintain min-heap property
    while (i != 0 && h->data[(i - 1) / 2].first > h->data[i].first) {
        pPair temp = h->data[i];
        h->data[i] = h->data[(i - 1) / 2];
        h->data[(i - 1) / 2] = temp;
        i = (i - 1) / 2;
    }
}

/**
 * pop - Remove and return the minimum element from the heap
 * Time complexity: O(log n)
 * 
 * @param h Pointer to the MinHeap structure
 * @return The minimum pPair value from the heap
 */
pPair pop(MinHeap* h) {
    // Store the minimum value
    pPair root = h->data[0];
    
    // Replace with last element and decrease size
    h->data[0] = h->data[--h->size];

    // Heapify down: maintain min-heap property
    int i = 0;
    while (1) {
        int smallest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        // Find the smallest among node and its children
        if (left < h->size && h->data[left].first < h->data[smallest].first)
            smallest = left;
        if (right < h->size && h->data[right].first < h->data[smallest].first)
            smallest = right;

        // If i is already the smallest, we're done
        if (smallest == i)
            break;

        // Swap with smallest child and continue heapifying
        pPair temp = h->data[i];
        h->data[i] = h->data[smallest];
        h->data[smallest] = temp;

        i = smallest;
    }

    return root;
}

/**
 * isEmpty - Check if the heap contains any elements
 * 
 * @param h Pointer to the MinHeap structure
 * @return 1 if empty, 0 otherwise
 */
int isEmpty(MinHeap* h) {
    return h->size == 0;
}

/**
 * freeHeap - Release all resources used by the heap
 * 
 * @param h Pointer to the MinHeap structure
 */
void freeHeap(MinHeap* h) {
    free(h->data);
    h->data = NULL;
    h->size = 0;
    h->capacity = 0;
}