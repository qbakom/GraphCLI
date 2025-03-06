#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

typedef enum {
    GRAPH_OK,
    GRAPH_ERROR_NULL,
    GRAPH_ERROR_INVALID_VERTEX,
    GRAPH_ERROR_MEMORY,
    GRAPH_ERROR_EDGE_EXISTS
} graph_error_t;

typedef struct Node {
    int vertex;
    struct Node* next;
} Node;

typedef struct Graph {
    int V;
    Node** adjLists;
    int* degrees;
    int isDirected; 
} Graph;

Graph* graph_create(int vertices, int isDirected);
graph_error_t graph_add_edge(Graph* graph, int src, int dest);
graph_error_t graph_has_edge(Graph* graph, int src, int dest);
void graph_display(Graph* graph);
graph_error_t graph_free(Graph* graph);
Graph* graph_generate_random(int vertices, int edgeCount, int isDirected);
Graph* handleUserInput();
void save_graph_to_file(Graph* graph, char* filename);
void generate_graph_image(char* dot_filename, char* img_filename);

#endif
