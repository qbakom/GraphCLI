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

//
Graph* processChatbotResponse() {
    Graph* graph = createGraph(parsed_nodes);
    for (int i = 0; i < edge_count; i++) {
        addEdge(graph, parsed_edges[i][0], parsed_edges[i][1]);
    }
    return graph;
}
//

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

            char userResponse[10];
            while (1) {
                printf("\nIs the generated graph parameters correct? (yes/no): ");
                scanf("%9s", userResponse);

                if (strcmp(userResponse, "yes") == 0) {
                    printf("\nGenerating the graph...\n\n");
                    graph = processChatbotResponse();
                    return graph;
                } else if (strcmp(userResponse, "no") == 0) {
                    printf("\nPlease enter a new prompt: ");
                    getchar(); 
                    fgets(userInput, sizeof(userInput), stdin);
                    userInput[strcspn(userInput, "\n")] = '\0'; 
                    sendQuery(userInput);
                } else {
                    printf("Invalid input. Please type yes or no.\n");
                }
            }
        }
        return graph;
    }
}

int main() {
    Graph* graph = handleUserInput();
    if (graph != NULL) {
        printGraph(graph);
    }
    return 0;
}
