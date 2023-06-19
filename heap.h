#ifndef __HEAP_H__
#define __HEAP_H__

#include "const.h"
#include "struct.h"

// Function to allocate heap
heap_t *heap_alloc(size_t size);

// Function to free all alocated memory for heap
void heap_free(heap_t *heap);

// Function to swap elements by indexes
void heap_swap(heap_t *heap, int index_a, int index_b);

// Function to push element with its cost to heap
void heap_push(heap_t *heap, int elem, int cost);

// Function to update element's cost in heap
void heap_update(heap_t *heap, int elem, int cost);

// Function to pop element from heap (returns name of the popped element)
int heap_pop(heap_t *heap);

#endif // __HEAP_H__
