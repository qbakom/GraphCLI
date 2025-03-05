#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "graph.h"
#include "ui.h"

Graph* createGraph(int vertices) {
    if (vertices <= 0) return NULL;
    
    Graph* graph = malloc(sizeof(Graph));
    if (!graph) return NULL;
    
    graph->numVertices = vertices;
    graph->adjLists = malloc(vertices * sizeof(Node*));
    
    if (!graph->adjLists) {
        free(graph);
        return NULL;
    }
    
    for (int i = 0; i < vertices; i++) {
        graph->adjLists[i] = NULL;
    }
    return graph;
}

void addEdge(Graph* graph, int src, int dest) {
    Node* newNode = malloc(sizeof(Node));
    newNode->vertex = dest;
    newNode->next = graph->adjLists[src];
    graph->adjLists[src] = newNode;

    newNode = malloc(sizeof(Node));
    newNode->vertex = src;
    newNode->next = graph->adjLists[dest];
    graph->adjLists[dest] = newNode;
}

void displayGraph(Graph* graph) {
    printf("\nGraph Adjacency List:\n");
    for (int v = 0; v < graph->numVertices; v++) {
        Node* temp = graph->adjLists[v];
        printf("Vertex %d: ", v);
        while (temp) {
            printf("%d -> ", temp->vertex);
            temp = temp->next;
        }
        printf("NULL\n");
    }
}

void freeGraph(Graph* graph) {
    if (!graph) return;
    
    for (int i = 0; i < graph->numVertices; i++) {
        Node* temp = graph->adjLists[i];
        while (temp) {
            Node* toFree = temp;
            temp = temp->next;
            free(toFree);
        }
    }
    free(graph->adjLists);
    free(graph);
}

void generateRandomGraph(Graph* graph) {
    srand(time(NULL));
    int maxEdges = graph->numVertices * (graph->numVertices - 1) / 2;
    int numEdges = rand() % maxEdges + 1;
    
    printf("Generating %d random edges...\n", numEdges);
    
    for (int i = 0; i < numEdges; i++) {
        int src = rand() % graph->numVertices;
        int dest = rand() % graph->numVertices;
        
        if (src != dest) {
            addEdge(graph, src, dest);
        }
    }
}

void userSpecifiedGraph(Graph* graph) {
    int numEdges, src, dest;
    
    printf("Enter number of edges: ");
    scanf("%d", &numEdges);
    
    if (numEdges < 0) {
        printf("Number of edges cannot be negative. Setting to 0.\n");
        numEdges = 0;
    }
    
    for (int i = 0; i < numEdges; i++) {
        printf("Enter edge %d (source destination): ", i+1);
        scanf("%d %d", &src, &dest);
        
        if (src >= 0 && src < graph->numVertices && 
            dest >= 0 && dest < graph->numVertices && 
            src != dest) {
            addEdge(graph, src, dest);
        } else {
            printf("Invalid edge. Vertex indices must be between 0 and %d and different from each other.\n", 
                   graph->numVertices - 1);
            i--;
        }
    }
}
