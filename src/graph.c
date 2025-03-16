#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <curl/curl.h>
#include "api.h"
#include "json_parser.h"
#include "graph.h"

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
    
    if (src == dest) return GRAPH_ERROR_INVALID_VERTEX;
    
    if (graph_has_edge(graph, src, dest) == GRAPH_OK)
        return GRAPH_ERROR_EDGE_EXISTS;
    
    Node* newNode = malloc(sizeof(Node));
    if (!newNode) return GRAPH_ERROR_MEMORY;
    
    newNode->vertex = dest;
    newNode->next = graph->adjLists[src];
    graph->adjLists[src] = newNode;
    graph->degrees[src]++;
    
    if (!graph->isDirected) {
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
        
        if (src != dest) {
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
    char buffer[256];
    
    printf("Choose program operation mode:\n");
    printf("1. Manual parameters input\n");
    printf("2. Chat mode with LM Studio\n");
    
    while (1) {
        printf("Enter choice (1 or 2): ");
        if (scanf("%d", &mode) != 1) {
            while (getchar() != '\n');
            printf("[!] Invalid input. Please enter 1 or 2.\n");
            continue;
        }
        
        if (mode == 1 || mode == 2) break;
        printf("[!] Invalid choice. Please enter 1 or 2.\n");
    }
    while (getchar() != '\n');

    if (mode == 1) {
        printf("Enter the number of vertices: ");
        if (scanf("%d", &vertices) != 1 || vertices <= 0) {
            while (getchar() != '\n');
            printf("[!] Invalid input. Using default value of 5 vertices.\n");
            vertices = 5;
        }
        while (getchar() != '\n');
        
        printf("Graph type:\n");
        printf("1. Directed (one-way edges)\n");
        printf("2. Undirected (two-way edges)\n");
        
        int graphTypeChoice;
        printf("Enter choice (1 or 2): ");
        if (scanf("%d", &graphTypeChoice) != 1 || 
           (graphTypeChoice != 1 && graphTypeChoice != 2)) {
            while (getchar() != '\n');
            printf("[!] Invalid input. Using default: undirected graph.\n");
            isDirected = 0;
        } else {
            isDirected = (graphTypeChoice == 1);
        }
        while (getchar() != '\n');
        
        printf("Graph generation:\n");
        printf("1. Manual edge input\n");
        printf("2. Random graph\n");
        
        int genChoice;
        printf("Enter choice (1 or 2): ");
        if (scanf("%d", &genChoice) != 1 || 
           (genChoice != 1 && genChoice != 2)) {
            while (getchar() != '\n');
            printf("[!] Invalid input. Using random graph generation.\n");
            genChoice = 2;
        }
        while (getchar() != '\n');

        Graph* graph = graph_create(vertices, isDirected);
        if (!graph) return NULL;
        
        if (genChoice == 1) {
            printf("Enter the number of edges: ");
            if (scanf("%d", &edges) != 1 || edges < 0) {
                while (getchar() != '\n');
                int defaultEdges = vertices * 2;
                printf("[!] Invalid input. Using default value of %d edges.\n", defaultEdges);
                edges = defaultEdges;
            }
            while (getchar() != '\n');

            int maxEdges = isDirected ? vertices * (vertices - 1) : vertices * (vertices - 1) / 2;
            if (edges > maxEdges) {
                printf("[!] Maximum possible edges for this graph: %d. Using this value.\n", maxEdges);
                edges = maxEdges;
            }
            
            if (edges == 0) {
                printf("[!] Cannot create a graph with 0 edges in manual mode. Using 1 edge.\n");
                edges = 1;
            }

            printf("\nInput format for edges: <source> <destination>\n");
            printf("Both source and destination must be integers between 0 and %d\n", vertices-1);
            printf("Example: '0 1' creates an edge from vertex 0 to vertex 1\n\n");

            int successful_edges = 0;
            while (successful_edges < edges) {
                int src = -1, dest = -1, count = 0;
                printf("Enter edge %d of %d [source destination]: ", successful_edges + 1, edges);
                
                if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
                    printf("[!] Error reading input. Try again.\n");
                    continue;
                }
                
                count = sscanf(buffer, "%d %d", &src, &dest);
                
                if (count != 2) {
                    printf("[!] Invalid format. Please enter TWO integers separated by space: <source> <destination>\n");
                    continue;
                }
                
                if (src < 0 || src >= vertices || dest < 0 || dest >= vertices || src == dest) {
                    printf("[!] Invalid edge. Source and destination must be different values between 0 and %d.\n", vertices-1);
                    continue;
                }
                
                graph_error_t result = graph_add_edge(graph, src, dest);
                if (result == GRAPH_OK) {
                    successful_edges++;
                    printf("[i] Edge %d -> %d added (%d/%d)\n", src, dest, successful_edges, edges);
                } else if (result == GRAPH_ERROR_EDGE_EXISTS) {
                    printf("[!] Edge already exists. Please enter a different edge.\n");
                } else {
                    printf("[!] Error adding edge. Please try again.\n");
                }
            }
        } else {
            printf("Enter the number of edges: ");
            if (scanf("%d", &edges) != 1 || edges < 0) {
                while (getchar() != '\n');
                int defaultEdges = vertices * 2;
                printf("[!] Invalid input. Using default value of %d edges.\n", defaultEdges);
                edges = defaultEdges;
            }
            while (getchar() != '\n');
            
            if (edges == 0) {
                printf("[i] Creating an empty graph with %d vertices and no edges.\n", vertices);
            } else {
                graph_free(graph);
                graph = graph_generate_random(vertices, edges, isDirected);
            }
        }
        
        return graph;
    } else {
        printf("\n=== Chat Mode ===\n");
        printf("Describe the graph you want to create in natural language.\n");
        printf("Example: 'Create a directed graph with 5 vertices and 7 edges'\n\n");
        
        printf("Your request: ");
        if (fgets(userInput, sizeof(userInput), stdin) == NULL) {
            printf("[!] Error reading input.\n");
            return NULL;
        }
        userInput[strcspn(userInput, "\n")] = 0;
        
        printf("\nSending request to LM Studio...\n");
        use_fallback = 0;
        sendQuery(userInput);
        
        if (use_fallback) {
            printf("[!] Failed to get a response from LM Studio.\n");
            printf("[!] Please check if LM Studio is running and try again.\n");
            return NULL;
        } else {
            finishJsonParsing();
            
            if (parsed_nodes <= 0) {
                printf("[!] Failed to parse valid graph from LM Studio response.\n");
                return NULL;
            }
        }
        
        if (parsed_nodes <= 0) {
            printf("[!] No valid graph data received.\n");
            return NULL;
        }
        
        Graph* graph = graph_create(parsed_nodes, 0);
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

void save_graph_to_file(Graph* graph, char* filename) {
    if (!graph || !filename) return;

    FILE* file = fopen(filename, "w");
    if (!file) {
        fprintf(stderr, "[!] Error: Could not open file %s for writing\n", filename);
        return;
    }

    fprintf(file, graph->isDirected ? "digraph G {\n" : "graph G {\n");

    for (int i = 0; i < graph->V; i++) {
        Node* edge = graph->adjLists[i];
        while (edge) {
            if (graph->isDirected) {
                fprintf(file, "    %d -> %d;\n", i, edge->vertex);
            } else if (i < edge->vertex) {
                fprintf(file, "    %d -- %d;\n", i, edge->vertex);
            }
            edge = edge->next;
        }
    }

    fprintf(file, "}\n");
    fclose(file);
    printf("[i] Graph saved to %s\n", filename);
}

void generate_graph_image(char* dot_filename, char* img_filename) {
    char command[512];
    snprintf(command, sizeof(command), "dot -Tpng %s -o %s", dot_filename, img_filename);
    int result = system(command);
    if (result == 0) {
        printf("[i] Graph image generated: %s\n", img_filename);
    } else {
        printf("[!] Graphviz not installed - visualization skipped\n");
        printf("[i] The DOT file is still available at: %s\n", dot_filename);
    }
}
