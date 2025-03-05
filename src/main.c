#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"
#include "ui.h"
#include "parser.h"
#include "utils.h"

int main() {
    int numVertices;
    int choice;
    Graph* graph;

    printf("Welcome to the Graph Generator!\n");
    printf("Enter the number of vertices: ");
    scanf("%d", &numVertices);

    if (!validateInput(numVertices)) {
        printf("Error: Number of vertices must be positive.\n");
        return 1;
    }

    graph = createGraph(numVertices);
    if (!graph) {
        handleError("Could not create graph");
        return 1;
    }

    printf("Choose generation mode:\n1. Random\n2. User-specified\n");
    scanf("%d", &choice);

    if (choice == 1) {
        generateRandomGraph(graph);
    } else if (choice == 2) {
        userSpecifiedGraph(graph);
    } else {
        printf("Error: Invalid choice.\n");
        freeGraph(graph);
        return 1;
    }

    displayGraph(graph);
    freeGraph(graph);
    return 0;
}
