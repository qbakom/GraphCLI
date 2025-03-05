#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

typedef struct Node {
    int vertex;
    struct Node* next;
} Node;

typedef struct Graph {
    int V;
    Node** adjLists;
} Graph;

Graph* createGraph(int vertices) {
    Graph* graph = malloc(sizeof(Graph));
    graph->V = vertices;
    graph->adjLists = malloc(vertices * sizeof(Node*));

    for (int i = 0; i < vertices; i++)
        graph->adjLists[i] = NULL;

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

void printGraph(Graph* graph) {
    for (int v = 0; v < graph->V; v++) {
        printf("Wierzchołek %d:", v);
        Node* temp = graph->adjLists[v];
        while (temp) {
            printf(" -> %d", temp->vertex);
            temp = temp->next;
        }
        printf("\n");
    }
}

Graph* handleUserInput() {
    int vertices, edges, choice;
    
    printf("Enter the number of vertices: ");
    scanf("%d", &vertices);

    while (vertices <= 0) {
        printf("[!] Number of vertices must be greater than 0.\n");
        printf("Enter the number of vertices: ");
        scanf("%d", &vertices);
    }


    Graph* graph = createGraph(vertices);

    printf("Choose graph type:\n");
    printf("1. Manual input\n");
    printf("2. Random graph\n");
    do {
        printf("Enter choice (1 or 2): ");
        scanf("%d", &choice);
        if (choice != 1 && choice != 2)
            printf("Error: Please enter 1 or 2.\n");
    } while (choice != 1 && choice != 2);

    printf("Enter the number of edges: ");
    scanf("%d", &edges);
    
    int maxEdges = vertices * (vertices - 1) / 2;
    if (edges > maxEdges) {
        printf("[!] Maximum possible edges for %d vertices is %d\n", vertices, maxEdges);
        edges = maxEdges;
        printf("[!] Using %d edges instead\n", edges);
    }

    if (choice == 1) {
        for (int i = 0; i < edges; i++) {
            int src, dest;
            printf("Enter edge %d [src dest]: ", i + 1);
            scanf("%d %d", &src, &dest);
            
            if (src < 0 || src >= vertices || dest < 0 || dest >= vertices) {
                printf("[!] Invalid vertices. Must be between 0 and %d\n", vertices - 1);
                i--;
                continue;
            }
            
            if (src == dest) {
                printf("[!] Self-loops are not allowed\n");
                i--;
                continue;
            }
            
            addEdge(graph, src, dest);
        }
    } else {

        srand(time(NULL));
        
        int addedEdges = 0;
        int** edgeMatrix = malloc(vertices * sizeof(int*));
        for (int i = 0; i < vertices; i++) {
            edgeMatrix[i] = calloc(vertices, sizeof(int));
        }
        
        while (addedEdges < edges) {
            int src = rand() % vertices;
            int dest = rand() % vertices;
            if (src != dest && edgeMatrix[src][dest] == 0) {
                addEdge(graph, src, dest);
                edgeMatrix[src][dest] = 1;
                edgeMatrix[dest][src] = 1;
                addedEdges++;
            }
        }
        
        for (int i = 0; i < vertices; i++) {
            free(edgeMatrix[i]);
        }
        free(edgeMatrix);
    }
    return graph;
}

void freeGraph(Graph* graph) {
    if (!graph) return;
    
    for (int v = 0; v < graph->V; v++) {
        Node* current = graph->adjLists[v];
        while (current) {
            Node* temp = current;
            current = current->next;
            free(temp);
        }
    }
    
    free(graph->adjLists);
    free(graph);
}

bool isEdgeBidirectional(Graph* graph, int src, int dest) {
    bool srcToDest = false;
    bool destToSrc = false;
    
    Node* temp = graph->adjLists[src];
    while (temp) {
        if (temp->vertex == dest) {
            srcToDest = true;
            break;
        }
        temp = temp->next;
    }
    
    temp = graph->adjLists[dest];
    while (temp) {
        if (temp->vertex == src) {
            destToSrc = true;
            break;
        }
        temp = temp->next;
    }
    
    return srcToDest && destToSrc;
}

bool testBidirection(Graph* graph) {
    printf("\nTesting graph bidirectionality...\n");
    
    for (int src = 0; src < graph->V; src++) {
        Node* temp = graph->adjLists[src];
        
        while (temp) {
            int dest = temp->vertex;
            
            if (!isEdgeBidirectional(graph, src, dest)) {
                printf("FAIL: Edge (%d -> %d) is not bidirectional!\n", src, dest);
                return false;
            }
            
            temp = temp->next;
        }
    }
    
    printf("SUCCESS: All edges are bidirectional! Graph is undirected.\n");
    return true;
}

int main() {
    int choice;
    
    printf("Choose an option:\n");
    printf("1. Create a new graph\n");
    printf("2. Run bidirectional graph test\n");
    printf("Enter choice (1 or 2): ");
    scanf("%d", &choice);
    
    if (choice == 1) {
        Graph* graph = handleUserInput();
        printGraph(graph);
        
        printf("\nWould you like to verify if the graph is bidirectional? (1: Yes, 0: No): ");
        int testChoice;
        scanf("%d", &testChoice);
        
        if (testChoice == 1) {
            testBidirection(graph);
        }
        
        freeGraph(graph);
    } 
    else if (choice == 2) {
        printf("Running bidirectional graph test...\n");
        
        Graph* testGraph = createGraph(5);
        
        addEdge(testGraph, 0, 1);
        addEdge(testGraph, 1, 2);
        addEdge(testGraph, 2, 3);
        addEdge(testGraph, 3, 4);
        addEdge(testGraph, 4, 0);
        
        printf("Test graph adjacency lists:\n");
        printGraph(testGraph);
        
        testBidirection(testGraph);
        
        freeGraph(testGraph);
    } 
    else {
        printf("Invalid choice!\n");
    }
    
    return 0;
}
