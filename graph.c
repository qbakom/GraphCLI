#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "include/api.h"
#include "include/json_parser.h"
#include <curl/curl.h>

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

extern int parsed_nodes;
extern int parsed_edges[100][2];
extern int edge_count;

Graph* graph_create(int vertices, int isDirected) {
    if (vertices <= 0) {
        fprintf(stderr, "[!] Invalid number of vertices\n");
        return NULL;
    }

    Graph* graph = malloc(sizeof(Graph));
    if (!graph) {
        fprintf(stderr, "[!] Memory allocation failed for graph\n");
        return NULL;
    }

    graph->V = vertices;
    graph->isDirected = isDirected;
    graph->adjLists = malloc(vertices * sizeof(Node*));
    if (!graph->adjLists) {
        fprintf(stderr, "[!] Memory allocation failed for adjacency lists\n");
        free(graph);
        return NULL;
    }

    graph->degrees = malloc(vertices * sizeof(int));
    if (!graph->degrees) {
        fprintf(stderr, "[!] Memory allocation failed for degrees array\n");
        free(graph->adjLists);
        free(graph);
        return NULL;
    }

    for (int i = 0; i < vertices; i++) {
        graph->adjLists[i] = NULL;
        graph->degrees[i] = 0;
    }

    return graph;
}

graph_error_t graph_free(Graph* graph) {
    if (!graph) return GRAPH_ERROR_NULL;

    for (int i = 0; i < graph->V; i++) {
        Node* current = graph->adjLists[i];
        while (current) {
            Node* temp = current;
            current = current->next;
            free(temp);
        }
    }
    
    free(graph->degrees);
    free(graph->adjLists);
    free(graph);
    
    return GRAPH_OK;
}

graph_error_t graph_has_edge(Graph* graph, int src, int dest) {
    if (!graph) return GRAPH_ERROR_NULL;
    if (src < 0 || src >= graph->V || dest < 0 || dest >= graph->V)
        return GRAPH_ERROR_INVALID_VERTEX;
    
    Node* current = graph->adjLists[src];
    while (current) {
        if (current->vertex == dest)
            return GRAPH_OK;
        current = current->next;
    }
    return GRAPH_ERROR_EDGE_EXISTS;
}

graph_error_t graph_add_edge(Graph* graph, int src, int dest) {
    if (!graph) return GRAPH_ERROR_NULL;
    if (src < 0 || src >= graph->V || dest < 0 || dest >= graph->V)
        return GRAPH_ERROR_INVALID_VERTEX;
    
    if (src == dest) return GRAPH_ERROR_INVALID_VERTEX; // No self-loops
    
    // Check if edge already exists
    if (graph_has_edge(graph, src, dest) == GRAPH_OK)
        return GRAPH_ERROR_EDGE_EXISTS;
    
    Node* newNode = malloc(sizeof(Node));
    if (!newNode) return GRAPH_ERROR_MEMORY;
    
    newNode->vertex = dest;
    newNode->next = graph->adjLists[src];
    graph->adjLists[src] = newNode;
    graph->degrees[src]++;
    
    // If bidirectional, add the other direction too
    if (!graph->isDirected) {
        // Check if reverse edge already exists
        if (graph_has_edge(graph, dest, src) == GRAPH_OK) {
            return GRAPH_OK;
        }
        
        newNode = malloc(sizeof(Node));
        if (!newNode) return GRAPH_ERROR_MEMORY;
        
        newNode->vertex = src;
        newNode->next = graph->adjLists[dest];
        graph->adjLists[dest] = newNode;
        graph->degrees[dest]++;
    }
    
    return GRAPH_OK;
}

void graph_display(Graph* graph) {
    if (!graph) {
        printf("[!] Graph is NULL\n");
        return;
    }
    
    printf("\nGraph with %d vertices (%s):\n", 
           graph->V, 
           graph->isDirected ? "directed" : "undirected");
           
    printf("Adjacency List:\n");
    for (int v = 0; v < graph->V; v++) {
        printf("%d (%d edges): ", v, graph->degrees[v]);
        Node* temp = graph->adjLists[v];
        while (temp) {
            printf("%d -> ", temp->vertex);
            temp = temp->next;
        }
        printf("NULL\n");
    }
}

Graph* graph_generate_random(int vertices, int edgeCount, int isDirected) {
    if (vertices <= 0) return NULL;
    
    Graph* graph = graph_create(vertices, isDirected);
    if (!graph) return NULL;
    
    srand(time(NULL));
    
    int maxPossibleEdges = isDirected ? 
        vertices * (vertices - 1) : 
        vertices * (vertices - 1) / 2;
    
    if (edgeCount > maxPossibleEdges) {
        edgeCount = maxPossibleEdges;
        printf("[!] Edge count reduced to maximum possible: %d\n", maxPossibleEdges);
    }
    
    int addedEdges = 0;
    while (addedEdges < edgeCount) {
        int src = rand() % vertices;
        int dest = rand() % vertices;
        
        if (src != dest) { // Avoid self-loops
            graph_error_t result = graph_add_edge(graph, src, dest);
            if (result == GRAPH_OK) {
                addedEdges++;
            }
        }
    }
    
    return graph;
}

Graph* handleUserInput() {
    int vertices, edges, mode, isDirected;
    char userInput[256];
    
    printf("Choose program operation mode:\n");
    printf("1. Manual parameters input\n");
    printf("2. Chat mode with LM Studio\n");
    
    while (1) {
        printf("Enter choice (1 or 2): ");
        if (scanf("%d", &mode) != 1) {
            printf("[!] Invalid input. Please enter 1 or 2.\n");
            while (getchar() != '\n'); // Clear input buffer
            continue;
        }
        
        if (mode == 1 || mode == 2) break;
        printf("[!] Invalid choice. Please enter 1 or 2.\n");
    }

    if (mode == 1) {
        // Manual mode
        printf("Enter the number of vertices: ");
        while (scanf("%d", &vertices) != 1 || vertices <= 0) {
            printf("[!] Invalid input. Please enter a positive integer: ");
            while (getchar() != '\n'); // Clear input buffer
        }
        
        printf("Graph type:\n");
        printf("1. Directed (one-way edges)\n");
        printf("2. Undirected (two-way edges)\n");
        int graphTypeChoice;
        while (1) {
            printf("Enter choice (1 or 2): ");
            if (scanf("%d", &graphTypeChoice) != 1 || 
               (graphTypeChoice != 1 && graphTypeChoice != 2)) {
                printf("[!] Invalid input. Please enter 1 or 2.\n");
                while (getchar() != '\n'); // Clear input buffer
                continue;
            }
            break;
        }
        isDirected = (graphTypeChoice == 1);
        
        printf("Graph generation:\n");
        printf("1. Manual edge input\n");
        printf("2. Random graph\n");
        int genChoice;
        while (1) {
            printf("Enter choice (1 or 2): ");
            if (scanf("%d", &genChoice) != 1 || 
               (genChoice != 1 && genChoice != 2)) {
                printf("[!] Invalid input. Please enter 1 or 2.\n");
                while (getchar() != '\n'); // Clear input buffer
                continue;
            }
            break;
        }

        Graph* graph = graph_create(vertices, isDirected);
        if (!graph) return NULL;
        
        if (genChoice == 1) {
            // Manual edge input
            printf("Enter the number of edges: ");
            while (scanf("%d", &edges) != 1 || edges < 0) {
                printf("[!] Invalid input. Please enter a non-negative integer: ");
                while (getchar() != '\n'); // Clear input buffer
            }

            int maxEdges = isDirected ? vertices * (vertices - 1) : vertices * (vertices - 1) / 2;
            if (edges > maxEdges) {
                printf("[!] Maximum possible edges for this graph: %d. Using this value.\n", maxEdges);
                edges = maxEdges;
            }

            for (int i = 0; i < edges; i++) {
                int src, dest;
                printf("Enter edge %d [source destination]: ", i + 1);
                while (scanf("%d %d", &src, &dest) != 2 || 
                       src < 0 || src >= vertices || 
                       dest < 0 || dest >= vertices || 
                       src == dest) {
                    printf("[!] Invalid edge. Source and destination must be different values between 0 and %d.\n", vertices-1);
                    printf("Enter edge %d [source destination]: ", i + 1);
                    while (getchar() != '\n'); // Clear input buffer
                }
                
                graph_error_t result = graph_add_edge(graph, src, dest);
                if (result != GRAPH_OK) {
                    i--; // Try again for this edge
                    if (result == GRAPH_ERROR_EDGE_EXISTS) {
                        printf("[!] Edge already exists. Please enter a different edge.\n");
                    } else {
                        printf("[!] Error adding edge. Please try again.\n");
                    }
                }
            }
        } else {
            // Random graph
            printf("Enter the number of edges: ");
            while (scanf("%d", &edges) != 1 || edges < 0) {
                printf("[!] Invalid input. Please enter a non-negative integer: ");
                while (getchar() != '\n'); // Clear input buffer
            }
            
            // Free the previously created graph and generate a random one
            graph_free(graph);
            graph = graph_generate_random(vertices, edges, isDirected);
        }
        
        return graph;
    } else {
        // Chat mode with LM Studio
        printf("\n=== Chat Mode ===\n");
        printf("Describe the graph you want to create in natural language.\n");
        printf("Example: 'Create a directed graph with 5 vertices and 7 edges'\n\n");
        
        while (getchar() != '\n'); // Clear input buffer
        printf("Your request: ");
        fgets(userInput, sizeof(userInput), stdin);
        userInput[strcspn(userInput, "\n")] = 0; // Remove newline
        
        printf("\nSending request to LM Studio...\n");
        sendQuery(userInput);
        
        if (parsed_nodes <= 0) {
            printf("[!] Failed to parse valid node count from LM Studio response.\n");
            return NULL;
        }
        
        // Default to undirected graph (can be enhanced to extract this from the prompt)
        Graph* graph = graph_create(parsed_nodes, 0); // 0 = undirected
        if (!graph) return NULL;
        
        printf("Creating graph with %d vertices and %d edges\n", parsed_nodes, edge_count);
        
        for (int i = 0; i < edge_count; i++) {
            int src = parsed_edges[i][0];
            int dest = parsed_edges[i][1];
            
            if (src < 0 || src >= parsed_nodes || dest < 0 || dest >= parsed_nodes || src == dest) {
                printf("[!] Skipping invalid edge: %d -> %d\n", src, dest);
                continue;
            }
            
            graph_error_t result = graph_add_edge(graph, src, dest);
            if (result != GRAPH_OK) {
                if (result == GRAPH_ERROR_EDGE_EXISTS) {
                    printf("[!] Skipping duplicate edge: %d -> %d\n", src, dest);
                } else {
                    printf("[!] Error adding edge: %d -> %d\n", src, dest);
                }
            }
        }
        
        return graph;
    }
}

int main() {
    Graph* graph = handleUserInput();
    
    if (graph) {
        graph_display(graph);
        graph_free(graph);
    } else {
        printf("[!] Failed to create graph\n");
    }
    
    return 0;
}
