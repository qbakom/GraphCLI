#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>


typedef struct Node {
    int vertex;
    struct Node* next;
} Node;

typedef struct Graph {
    int V;
    Node** adjLists;
} Graph;

Graph* createGraph(int vertices);
void addEdge(Graph* graph, int src, int dest);
void freeGraph(Graph* graph);

bool hasEdge(Graph* graph, int src, int dest) {
    Node* temp = graph->adjLists[src];
    while (temp) {
        if (temp->vertex == dest)
            return true;
        temp = temp->next;
    }
    return false;
}

void testBidirectionalEdge() {
    printf("Test: Bidirectional edge addition...");
    
    Graph* g = createGraph(3);
    addEdge(g, 0, 1);
    
    assert(hasEdge(g, 0, 1) && "Edge 0->1 should exist");
    assert(hasEdge(g, 1, 0) && "Edge 1->0 should exist (bidirectional)");
    
    freeGraph(g);
    printf("PASSED\n");
}

void testMultipleEdges() {
    printf("Test: Multiple bidirectional edges...");
    
    Graph* g = createGraph(4);
    addEdge(g, 0, 1);
    addEdge(g, 1, 2);
    addEdge(g, 2, 3);
    
    assert(hasEdge(g, 0, 1) && hasEdge(g, 1, 0));
    assert(hasEdge(g, 1, 2) && hasEdge(g, 2, 1));
    assert(hasEdge(g, 2, 3) && hasEdge(g, 3, 2));
    
    freeGraph(g);
    printf("PASSED\n");
}

void testNoUnidirectionalEdges() {
    printf("Test: No unidirectional edges...");
    
    Graph* g = createGraph(5);
    addEdge(g, 0, 1);
    addEdge(g, 1, 2);
    addEdge(g, 2, 3);
    addEdge(g, 3, 4);
    
    for (int i = 0; i < g->V; i++) {
        Node* temp = g->adjLists[i];
        while (temp) {
            int j = temp->vertex;
            assert(hasEdge(g, j, i) && "All edges should be bidirectional");
            temp = temp->next;
        }
    }
    
    freeGraph(g);
    printf("PASSED\n");
}

int main() {
    printf("Running bidirectional graph tests...\n");
    
    testBidirectionalEdge();
    testMultipleEdges();
    testNoUnidirectionalEdges();
    
    printf("\nAll tests passed! Your graph implementation is bidirectional (undirected).\n");
    return 0;
}
