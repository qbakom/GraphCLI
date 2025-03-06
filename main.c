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
            char png_filename[260];
            printf("Enter filename (without extension): ");
            scanf("%s", filename);

            save_graph_to_file(graph, filename);
            snprintf(png_filename, sizeof(png_filename), "%s.png", filename);
            generate_graph_image(filename, png_filename);
        }
        
        graph_free(graph);
    } else {
        printf("[!] Failed to create graph\n");
    }

    return 0;
}