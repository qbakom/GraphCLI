#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include "include/graph.h"
#include "include/api.h"
#include "include/json_parser.h"

int main() {
    struct stat st = {0};
    if (stat("graphs", &st) == -1) {
        mkdir("graphs", 0700);
    }
  
    Graph* graph = handleUserInput();

    if (graph) {
        graph_display(graph);
        
        char choice[10];
        printf("Do you want to save the graph to a file? (yes/no): ");
        scanf("%s", choice);

        if (strcmp(choice, "yes") == 0) {
            char filename[256]; 
            printf("Enter filename (without extension): ");
            scanf("%s", filename);

            save_graph_to_file(graph, filename);
            
            char png_filename[260];
            snprintf(png_filename, sizeof(png_filename), "%s.png", filename);
            generate_graph_image(filename, png_filename);
        }
        
        graph_free(graph);
    } else {
        printf("[!] Failed to create graph\n");
    }

    return 0;
}