#ifndef __GRAPH_H__
#define __GRAPH_H__

#include "const.h"
#include "struct.h"

// Function to allocate new graph
graph_t* allocate_graph();

// Function to enlarge graph
_Bool enlarge_graph(graph_t *graph);

// Function to delete graph and free allocated memory
void free_graph(graph_t **graph);

// Function to load graph from text file
_Bool load_txt(const char *fname, graph_t *graph);

// Function to open file and copy to memory
char *read_file(const char *fname, int *size);

// Function to create graph's nodes
_Bool create_nodes(graph_t *graph);

// Function to save graph to text file
_Bool save_txt(dijkstra_t *dijkstra, const char *fname);

// Function to buffer number for saving into text file
void bufferize_number(char **buffer, int *pos, int number);

#endif // __GRAPH_H__
