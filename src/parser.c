#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parser.h"
#include "graph.h"

int parseCommand(const char* command, Graph* graph) {
    if (strstr(command, "add edge") != NULL) {
        int vertices[2];
        if (extractVertices(command, vertices, 2) == 2) {
            addEdge(graph, vertices[0], vertices[1]);
            return 1;
        }
    } else if (strstr(command, "display") != NULL) {
        displayGraph(graph);
        return 1;
    }
    
    return 0;
}

int extractVertices(const char* command, int* vertices, int maxVertices) {
    int count = 0;
    const char* ptr = command;
    
    while (*ptr && !isdigit(*ptr)) ptr++;
    
    while (*ptr && count < maxVertices) {
        if (isdigit(*ptr)) {
            vertices[count++] = atoi(ptr);
            
            while (*ptr && isdigit(*ptr)) ptr++;
        } else {
            ptr++;
        }
    }
    
    return count;
}
