/*
Some parts were inspired by code given by prof. Jan Faigl at lecture
*/

#include "dijkstra.h"
#include "graph.h"
#include "heap.h"

// Function to alocate Dijkstra's element
void* dijkstra_init(void) {
    dijkstra_t *dijkstra = malloc(sizeof(dijkstra_t));
    if (!dijkstra) {    // Memory allocation control
        fprintf(stderr, MEM_ERR_MSG);
        return NULL;
    }

    dijkstra->graph = allocate_graph();
    if (!dijkstra->graph) { // Memory allocation control
        fprintf(stderr, MEM_ERR_MSG);
        free(dijkstra);
        return NULL;
    }

    dijkstra->prevs = NULL;
    dijkstra->costs = NULL;

    return dijkstra;
}


// Function to free all alocated memory
void dijkstra_free(void *dij) {
    dijkstra_t *dijkstra = (dijkstra_t*)dij;
    if (dijkstra) {
        if (dijkstra->graph) {
            free_graph(&dijkstra->graph);
        }
        if (dijkstra->prevs) {
            free(dijkstra->prevs);
        }
        if (dijkstra->costs) {
            free(dijkstra->costs);
        }
        free(dijkstra);
    }
}
 

// Function to read graph from text file
_Bool dijkstra_load_graph(const char *filename, void *dij) {
    dijkstra_t *dijkstra = (dijkstra_t*)dij;
    if (!dijkstra || !dijkstra->graph) {  // Data existance control
        return false;
    }

    // Load edges from file
    if (!load_txt(filename, dijkstra->graph)) { // Loaded data control
        dijkstra_free(dijkstra);
        return false;
    }

    size_t size = dijkstra->graph->node_count;  // Number of nodes
    dijkstra->prevs = malloc(size * sizeof(int));
    dijkstra->costs = malloc(size * sizeof(int));
    if (!dijkstra->prevs || !dijkstra->costs) { // Memory allocation control
        dijkstra_free(dijkstra);
        return false;
    }

    for (int i = 0; i < size; ++i) {    // Initialization
        dijkstra->prevs[i] = NONE;
        dijkstra->costs[i] = NONE;
    }

    return true;
}


// Function to find the shortest paths between nodes in graph
_Bool dijkstra_solve(void *dij, int label) {
    dijkstra_t *dijkstra = (dijkstra_t*)dij;
    if (!dijkstra || label < 0 || label > dijkstra->graph->node_count - 1) {
        return false;
    }
    
    heap_t *heap = heap_alloc(dijkstra->graph->node_count);
    if (!heap) {    // Heap initialization control
        return false;
    }

    dijkstra->prevs[label] = 0; // Start node cost
    dijkstra->costs[label] = 0; // Mark start node as visited  
    heap_push(heap, label, 0);   // Push start node to heap

    while (heap->count) {   // While heap isn't empty
        int from_pos = heap_pop(heap);  // Node with minimal cost...
        node_t *from = &(dijkstra->graph->nodes[from_pos]); //... and its element
        for (int i = 0; i < from->edge_count; ++i) {    // Check all node edges
            edge_t *edge = &(dijkstra->graph->edges[from->edge_pos + i]);
            int to_pos = edge->to;
            int cur_cost = dijkstra->costs[from_pos] + edge->cost;
            if (dijkstra->prevs[to_pos] == NONE) {
                // Node haven't been visited yet              
                dijkstra->prevs[to_pos] = from_pos;
                dijkstra->costs[to_pos] = cur_cost;
                heap_push(heap, to_pos, cur_cost); 
            } else if (cur_cost < dijkstra->costs[to_pos]) {
                // Shorter way was found  
                dijkstra->prevs[to_pos] = from_pos;
                dijkstra->costs[to_pos] = cur_cost;
                heap_update(heap, to_pos, cur_cost); 
            }
        }
    }

    dijkstra->prevs[label] = NONE; // Delete start node's previous node
    heap_free(heap);
    return true;
} 


// Function to save solution to text file
_Bool dijkstra_save_path(const void *dij, const char *filename) {
    dijkstra_t *dijkstra = (dijkstra_t*)dij;
    if (!dijkstra) {
        return false;
    }

    if (!save_txt(dijkstra, filename)) {    // Writed data control
        return false;
    }

    return true;
}


// Function to read graph directly from array
// (should not be used in HW10B)
_Bool dijkstra_set_graph(int length, int edges[][3], void *dij) {
    dijkstra_t *dijkstra = (dijkstra_t*)dij;
    if (!dijkstra || !dijkstra->graph || !dijkstra->graph->edges) {
        return false;
    }

    for (int i = 0; i < length; ++i) {
        dijkstra->graph->edges[i].from = edges[i][0];
        dijkstra->graph->edges[i].to = edges[i][1];
        dijkstra->graph->edges[i].cost = edges[i][2];
        dijkstra->graph->edge_count++;
    }

    return create_nodes(dijkstra->graph);
}


// Function to get solution in the array
// (should not be used in HW10B)
_Bool dijkstra_get_solution(const void *dij, int length, int solution[][3]) {
    dijkstra_t *dijkstra = (dijkstra_t*)dij;
    if (!dijkstra) {
        return false;
    }

    for (int i = 0; i < length; ++i) {
        solution[i][0] = i;
        solution[i][1] = dijkstra->costs[i];
        solution[i][2] = dijkstra->prevs[i];
    }

    return true;
}
