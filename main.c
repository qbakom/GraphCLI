#include <stdio.h>
#include <string.h>
#include "include/graph.h"
#include "include/api.h"
#include "include/json_parser.h"

int main() {
    Graph* graph = handleUserInput();

    if (graph) {
        graph_display(graph);
        
        char choice[10];
        printf("Do you want to save the graph to a file? (yes/no): ");
        scanf("%s", choice);

        if (strcmp(choice, "yes") == 0) {
            char filename[256];
            printf("Enter filename: ");
            scanf("%255s", filename);

            save_graph_to_file(graph, filename);
        }
        
        graph_free(graph);
    } else {
        printf("[!] Failed to create graph\n");
    }

    return 0;
}