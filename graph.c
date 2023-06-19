/*
Use mmap to copy file content into memory inspired by:
https://lemire.me/blog/2012/06/26/which-is-fastest-read-fread-ifstream-or-mmap/

Bufferization idea for output inspired by Clifford answer it this thread: 
https://stackoverflow.com/questions/41210227/fastest-way-to-write-integer-to-file-in-c
*/

#include "graph.h"

// Function to allocate new graph
graph_t* allocate_graph() {
    graph_t *graph = malloc(sizeof(graph_t));
    if (!graph) {   // Memory allocation control
        fprintf(stderr, MEM_ERR_MSG);
        return NULL;
    }

    graph->size = DEF_SIZE;
    graph->edge_count = 0;
    graph->node_count = 0;
    graph->edges = malloc(graph->size * sizeof(edge_t));
    if (!graph->edges) {    // Memory allocation control
        fprintf(stderr, MEM_ERR_MSG);
        free_graph(&graph);
    }

    return graph;
}


// Function to enlarge graph
_Bool enlarge_graph(graph_t *graph) {
    graph->size = graph->size < LIM_SIZE ? graph->size * 10 : graph->size << 1;
    
    edge_t *tmp = realloc(graph->edges, graph->size * sizeof(edge_t));
    if (!tmp) {     // Memory reallocation control
        fprintf(stderr, MEM_ERR_MSG);
        return false;
    }
    graph->edges = tmp;

    return true;
}


// Function to delete graph and free allocated memory
void free_graph(graph_t **graph) {
    if (*graph) {
        if ((*graph)->edges) {
            free((*graph)->edges);
        }
        if ((*graph)->nodes) {
            free((*graph)->nodes);
        }
        free(*graph);
    }

    (*graph) = NULL;
}


// Function to load graph from text file
_Bool load_txt(const char *fname, graph_t *graph) {
    int size;
    char *buffer = read_file(fname, &size); // Open file and copy data to memory
    if (!buffer) {
        return false;
    }

    int ascii[128];  // Digits char-int transformation
    for (int i = 0; i < 128; ++i) {
        ascii[i] = (i >= '0' && i <= '9') ? i - '0' : NONE;
    }

    _Bool ret = true;
    for (int pos = 0; pos < size; ) {   // Read from buffer
        edge_t edge = {.from = NONE, .to = NONE, .cost = NONE};
        for (int i = 0; i < 3; ++i) {   // Read three numbers for one edge
            int number = 0;
            while (ascii[(int)buffer[pos++]] != NONE) {
                number = number * 10 + (ascii[(int)buffer[pos - 1]]);
            }
            if (i != 2 && number >= graph->node_count) {
                graph->node_count = number + 1;
            }
            *(&(edge.from) + i) = number;
        }

        if (graph->edge_count == graph->size) {  // Available space control
            if (!enlarge_graph(graph)) {    // Memory reallocation control
                ret = false;
                break;
            }
        }
        graph->edges[(graph->edge_count)++] = edge;
    }

    edge_t closing_edge = {.from = NONE, .to = NONE, .cost = NONE};
    graph->edges[graph->edge_count] = closing_edge;

    munmap(buffer, size); // Free used memory
    return ret ? create_nodes(graph) : ret;
}


// Function to open file and copy to memory
char *read_file(const char *fname, int *size) {
    char *buffer;

    int file = open(fname, O_RDONLY);
    if (file == NONE) { // Opened file control
        fprintf(stderr, FILE_OPEN_ERR_MSG);
        return NULL;
    }

    struct stat data;
    if (fstat(file, &data) == NONE) {   // File information control
        fprintf(stderr, FILE_OPEN_ERR_MSG);
        *size = 0;
    } else {
        *size = data.st_size;   // Size of file
    }

    buffer = mmap(NULL, *size, PROT_READ, MAP_PRIVATE, file, 0);
    if (buffer == MAP_FAILED) { // Copied memory control
        fprintf(stderr, MEM_ERR_MSG);
        buffer = NULL;
    }

    if (close(file) == NONE) {  // Closed file control
        fprintf(stderr, FILE_CLOSE_ERR_MSG);
    }

    return buffer;
}


// Function to create graph's nodes
_Bool create_nodes(graph_t *graph) {
    graph->nodes = malloc(graph->node_count * sizeof(node_t));
    if (!graph->nodes) {    // Memory allocation control
        fprintf(stderr, MEM_ERR_MSG);
        free_graph(&graph);
        return false;
    }

    int pos = 0;
    for (int i = 0; i < graph->node_count; ++i) {
        node_t node = {.edge_count = 0, .edge_pos = NONE};
        while (i == graph->edges[pos].from) {   // Count node's appearances
            node.edge_pos = node.edge_count ? node.edge_pos : pos;
            node.edge_count++;
            pos++;
        }
        graph->nodes[i] = node; // Write node data
    }

    return true;
}


// Function to save graph to text file
_Bool save_txt(dijkstra_t *dijkstra, const char *fname) {
    char *buffer = malloc(dijkstra->graph->node_count * 3 * 10);   // Buffer for writing in file
    if (!buffer) {    // Memory allocation control
        fprintf(stderr, MEM_ERR_MSG);
        return false;
    }

    int file = open(fname, O_WRONLY | O_CREAT, 0644);
    if (file == NONE) { // Opened file control
        fprintf(stderr, FILE_OPEN_ERR_MSG);
        free(buffer);
        return false;
    }

    int buffer_pos = 0;
    for (int node_pos = 0; node_pos < dijkstra->graph->node_count; ++node_pos) {
        bufferize_number(&buffer, &buffer_pos, node_pos);                  // Node's name
        buffer[buffer_pos++] = WSC;                                        // Whitespace
        bufferize_number(&buffer, &buffer_pos, dijkstra->costs[node_pos]); // Node's cost
        buffer[buffer_pos++] = WSC;                                        // Whitespace
        bufferize_number(&buffer, &buffer_pos, dijkstra->prevs[node_pos]); // Node's prev node
        buffer[buffer_pos++] = EOL;                                        // New line
    }

    write(file, buffer, buffer_pos);  // Write buffer to file
    
    if (close(file) == NONE) {  // Closed file control
        fprintf(stderr, FILE_CLOSE_ERR_MSG);
    }

    free(buffer);      // Final buffer cleaning
    return true;
}


// Function to buffer number for saving into text file
void bufferize_number(char **buffer, int *pos, int number) {
    if (number == NONE) {
        (*buffer)[(*pos)++] = NEG;          // Write sign
        (*buffer)[(*pos)++] = DIGITS[1];    // Write digit
        return;
    }

    // Count digits in number
    int digits = 0;
    int tmp = number;
    do {
        digits++;
        tmp /= 10;
    } while (tmp);

    // Write each number's digit into buffer 
    for(int i = digits - 1; i >= 0; --i) {
        (*buffer)[(*pos) + i] = DIGITS[number % 10];
        number /= 10;
    }
    (*pos) += digits;
}
