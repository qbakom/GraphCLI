#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Edge {
    int dest;
    struct Edge* next;
} Edge;

typedef struct Graph {
    int num_vertices;
    bool is_bidirectional;
    Edge** adj_lists;
} Graph;

Graph* create_graph(int num_vertices, bool is_bidirectional);
void add_edge(Graph* graph, int src, int dest);
void generate_random_graph(Graph* graph, double edge_density);
void print_graph(Graph* graph);
void free_graph(Graph* graph);
void save_graph_to_file(Graph* graph, const char* filename);

#endif
