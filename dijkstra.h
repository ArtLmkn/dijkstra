#ifndef __DIJKSTRA_H__
#define __DIJKSTRA_H__

#include "const.h"
#include "struct.h"

// Function to alocate Dijkstra's element
void* dijkstra_init(void);

// Function to free all alocated memory
void dijkstra_free(void *dij);
 
// Function to read graph from text file
_Bool dijkstra_load_graph(const char *filename, void *dij);
 
// Function to find thw shortest paths between nodes in graph
_Bool dijkstra_solve(void *dij, int label);
 
// Function to save solution to text file
_Bool dijkstra_save_path(const void *dij, const char *filename);

// Function to read graph directly from array
// (should not be used in HW10B)
_Bool dijkstra_set_graph(int length, int edges[][3], void *dij);

// Function to get solution in the array
// (should not be used in HW10B)
_Bool dijkstra_get_solution(const void *dij, int length, int solution[][3]);

#endif
