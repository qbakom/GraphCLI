#include "graph.h"
#include <time.h>

Graph* create_graph(int num_vertices, bool is_bidirectional) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    if (!graph) return NULL;
    
    graph->num_vertices = num_vertices;
    graph->is_bidirectional = is_bidirectional;
    graph->adj_lists = (Edge**)malloc(num_vertices * sizeof(Edge*));
    
    if (!graph->adj_lists) {
        free(graph);
        return NULL;
    }
    
    for (int i = 0; i < num_vertices; i++) {
        graph->adj_lists[i] = NULL;
    }
    
    return graph;
}

void add_edge(Graph* graph, int src, int dest) {
    if (!graph || src < 0 || dest < 0 || src >= graph->num_vertices || dest >= graph->num_vertices)
        return;
    
    Edge* new_edge = (Edge*)malloc(sizeof(Edge));
    if (!new_edge) return;
    
    new_edge->dest = dest;
    new_edge->next = graph->adj_lists[src];
    graph->adj_lists[src] = new_edge;
    
    if (graph->is_bidirectional) {
        Edge* new_edge_back = (Edge*)malloc(sizeof(Edge));
        if (!new_edge_back) return;
        
        new_edge_back->dest = src;
        new_edge_back->next = graph->adj_lists[dest];
        graph->adj_lists[dest] = new_edge_back;
    }
}

void generate_random_graph(Graph* graph, double edge_density) {
    if (!graph) return;
    
    srand(time(NULL));
    
    for (int i = 0; i < graph->num_vertices; i++) {
        for (int j = graph->is_bidirectional ? i + 1 : 0; j < graph->num_vertices; j++) {
            if (i == j) continue;
            
            if ((double)rand() / RAND_MAX < edge_density) {
                add_edge(graph, i, j);
            }
        }
    }
}

void print_graph(Graph* graph) {
    if (!graph) return;
    
    printf("Graph with %d vertices (%s):\n", 
           graph->num_vertices, 
           graph->is_bidirectional ? "bidirectional" : "unidirectional");
    
    for (int i = 0; i < graph->num_vertices; i++) {
        printf("Vertex %d:", i);
        Edge* edge = graph->adj_lists[i];
        
        while (edge) {
            printf(" → %d", edge->dest);
            edge = edge->next;
        }
        printf("\n");
    }
}

void free_graph(Graph* graph) {
    if (!graph) return;
    
    for (int i = 0; i < graph->num_vertices; i++) {
        Edge* current = graph->adj_lists[i];
        while (current) {
            Edge* temp = current;
            current = current->next;
            free(temp);
        }
    }
    
    free(graph->adj_lists);
    free(graph);
}

void save_graph_to_file(Graph* graph, const char* filename) {
    if (!graph || !filename) return;
    
    FILE* file = fopen(filename, "w");
    if (!file) return;
    
    fprintf(file, "%d %d\n", graph->num_vertices, graph->is_bidirectional);
    
    for (int i = 0; i < graph->num_vertices; i++) {
        Edge* edge = graph->adj_lists[i];
        while (edge) {
            fprintf(file, "%d %d\n", i, edge->dest);
            edge = edge->next;
        }
    }
    
    fclose(file);
}
