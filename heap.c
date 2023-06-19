/*
Some parts were inspired by code given by prof. Jan Faigl at lecture
*/

#include "heap.h"

// Function to allocate heap
heap_t *heap_alloc(size_t size) {
    heap_t *heap = malloc(sizeof(heap_t));
    if (!heap) {    // Memory allocation control
        return NULL;
    }

    heap->elems = malloc(sizeof(int) * size);
    heap->costs = malloc(sizeof(int) * size);
    heap->idxs = malloc(sizeof(int) * size);
    if (!heap->elems || !heap->costs || !heap->idxs) { // Memory allocation control
        fprintf(stderr, MEM_ERR_MSG);
        heap_free(heap);
        return NULL;
    }

    for (int i = 0; i < size; ++i) {    // Indexes array initialization
        heap->idxs[i] = NONE;
    }

    heap->count = 0;
    heap->size = size;

    return heap;
}


// Function to free all alocated memory for heap
void heap_free(heap_t *heap) {
    if (heap) {
        if (heap->elems) { 
            free(heap->elems);
        }
        if (heap->costs) { 
            free(heap->costs);
        }
        if (heap->idxs) { 
            free(heap->idxs);
        }
        free(heap);
   }
}


// Function to swap elements by indexes
void heap_swap(heap_t *heap, int a, int b) {
    int tmp;  // Cache variable

    // Swap elements indexes
    heap->idxs[heap->elems[a]] = b;
    heap->idxs[heap->elems[b]] = a;

    // Swap elements names
    tmp = heap->elems[a];
    heap->elems[a] = heap->elems[b];
    heap->elems[b] = tmp;

    // Swap elements costs
    tmp = heap->costs[a];
    heap->costs[a] = heap->costs[b];
    heap->costs[b] = tmp;
}


// Function to push element with its cost to heap
void heap_push(heap_t *heap, int elem, int cost) {
    int cur = heap->count;
    heap->elems[cur] = elem;
    heap->costs[cur] = cost;
    heap->idxs[elem] = cur;
    heap->count++;

    // Promote element up if needed
    while (cur > 0) {
        int parent = PARENT(cur);
        if (heap->costs[parent] > heap->costs[cur]) {
            heap_swap(heap, parent, cur);
            cur = parent;
        } else {
            break;
        }
    }
}


// Function to update element's cost in heap
void heap_update(heap_t *heap, int elem, int cost) {
    int cur = heap->idxs[elem];
    heap->costs[cur] = cost;

    // Promote element up if needed
    while (cur > 0) {
        int parent = PARENT(cur);
        if (heap->costs[parent] > heap->costs[cur]) {
            heap_swap(heap, parent, cur);
            cur = parent;
        } else {
            break;
        }
    }
}


// Function to pop element from heap (returns name of the popped element)
int heap_pop(heap_t *heap){
    if (!heap->count) { // Nothing to pop
        return NONE;
    }

    int popped_elem = heap->elems[0];
    heap->idxs[popped_elem] = NONE;
    heap->count--;

    heap->elems[0] = heap->elems[heap->count];
    heap->costs[0] = heap->costs[heap->count];
    heap->idxs[heap->elems[0]] = 0;

    // Demote element down if needed
    int cur = 0;
    int left, right, min;
    while (CHILD(cur) < heap->count) {
        left = CHILD(cur);
        right = left + 1 < heap->count ? left + 1 : 0;
        min = (right && heap->costs[right] < heap->costs[left]) ? right : left;

        if (heap->costs[cur] > heap->costs[min]) {
            heap_swap(heap, cur, min);
            cur = min;
        } else {
            break;
        }
    }

    return popped_elem;
}
