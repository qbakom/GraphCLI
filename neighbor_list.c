#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/api.h"
#include "include/json_parser.h"
#include <curl/curl.h>

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
    int vertices, edges, choice, mode;
    char userInput[256];
    char response[1024] = {0};
    
    printf("Choose program operation mode:\n");
    printf("1. Manual parameters input\n");
    printf("2. Conversation with chatbot to specify parameters\n");
    do {
        printf("Enter choice (1 or 2): ");
        scanf("%d", &mode);
        if (mode != 1 && mode != 2)
            printf("Error: Please enter 1 or 2.\n");
    } while (mode != 1 && mode != 2);

    if (mode == 1) {
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

        if (choice == 1) {
            printf("Enter the number of edges: ");
            scanf("%d", &edges);

            for (int i = 0; i < edges; i++) {
                int src, dest;
                printf("Enter edge %d [src dest]: ", i + 1);
                scanf("%d %d", &src, &dest);
                addEdge(graph, src, dest);
            }
        } else {
            printf("Enter the number of edges: ");
            scanf("%d", &edges);

            for (int i = 0; i < edges; i++) {
                int src = rand() % vertices;
                int dest = rand() % vertices;
                if (src != dest) {
                    addEdge(graph, src, dest);
                }
            }
        }
        return graph;
    } else {
        Graph* graph = NULL;
        while (graph == NULL) {
            printf("Write your prompt: "); 
            getchar();
            fgets(userInput, sizeof(userInput), stdin);
            userInput[strcspn(userInput, "\n")] = 0;
                        
            sendQuery(userInput);
        }
        return graph;
    }
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
    Graph* graph = handleUserInput();
    if (graph != NULL) {
        printGraph(graph);
    }
    return 0;
}
