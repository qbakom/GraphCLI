#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include "graph.h"
#include "api.h"
#include "json_parser.h"

int main() {
    
    // struct stat st = {0};            Unix/Linux: mkdir(path, permissions) - Takes two arguments
    //                                  Windows: mkdir(path) - Takes only one argument
    // if (stat("output", &st) == -1) {
    //     mkdir("output", 0700);
    // }
    // besides Makefile is doing it anyways
  
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
            
            char filepath[512];
            snprintf(filepath, sizeof(filepath), "output/%s", filename);

            save_graph_to_file(graph, filepath);
            
            char png_filename[512];
            snprintf(png_filename, sizeof(png_filename), "output/%s.png", filename);
            generate_graph_image(filepath, png_filename);
        } else {
            time_t now = time(NULL);
            struct tm *t = localtime(&now);
            char auto_filename[256];
            
            snprintf(auto_filename, sizeof(auto_filename), "output/graph_%04d%02d%02d_%02d%02d%02d", 
                     t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
                     t->tm_hour, t->tm_min, t->tm_sec);
            
            save_graph_to_file(graph, auto_filename);
            
            char png_filename[512];
            snprintf(png_filename, sizeof(png_filename), "%s.png", auto_filename);
            generate_graph_image(auto_filename, png_filename);
            
            printf("[i] Graph automatically saved to %s\n", auto_filename);
        }
        
        graph_free(graph);
    } else {
        printf("[!] Graph creation failed. Please try again.\n");
    }

    return 0;
}