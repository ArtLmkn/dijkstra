#ifndef __STRUCT_H__
#define __STRUCT_H__

// Edge structure
typedef struct {
   int from;
   int to;
   int cost;
} edge_t;

// Node structure
typedef struct {
   int edge_pos;
   int edge_count;
} node_t;

// Graph structure
typedef struct {
   edge_t *edges;
   node_t *nodes;
   size_t edge_count;
   size_t node_count;
   size_t size;
} graph_t;

// Dijkstra structure
typedef struct {
   graph_t *graph;
   int *prevs;
   int *costs;
} dijkstra_t;

// Heap structure
typedef struct {
   int *elems;
   int *costs;
   int *idxs;
   int count;
   int size;
} heap_t;

#endif // __STRUCT_H__
