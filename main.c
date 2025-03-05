#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "graph.h"
#include "llm_client.h"

#define MAX_INPUT 100
#define MAX_RESPONSE 8192

void ordered_mode();
void chat_mode();
void clear_input_buffer();

int main() {
    int mode;
    
    printf("Graph Generation Program\n");
    printf("======================\n");
    printf("Select mode:\n");
    printf("1. Ordered Mode (parameter-based)\n");
    printf("2. Chat Mode (natural language with LLM)\n");
    printf("Enter choice (1 or 2): ");
    
    if (scanf("%d", &mode) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    clear_input_buffer();
    
    switch (mode) {
        case 1:
            ordered_mode();
            break;
        case 2:
            chat_mode();
            break;
        default:
            printf("Invalid choice\n");
            break;
    }
    
    return 0;
}

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void ordered_mode() {
    int num_vertices;
    bool is_bidirectional;
    int generation_mode;
    char filename[100];
    
    printf("\nOrdered Mode\n");
    printf("===========\n");
    
    printf("Enter number of vertices: ");
    if (scanf("%d", &num_vertices) != 1 || num_vertices <= 0) {
        printf("Invalid number of vertices\n");
        return;
    }
    
    printf("Bidirectional graph? (1 for yes, 0 for no): ");
    int bidirectional_choice;
    if (scanf("%d", &bidirectional_choice) != 1) {
        printf("Invalid choice\n");
        return;
    }
    is_bidirectional = bidirectional_choice != 0;
    
    Graph* graph = create_graph(num_vertices, is_bidirectional);
    if (!graph) {
        printf("Failed to create graph\n");
        return;
    }
    
    printf("Select graph generation mode:\n");
    printf("1. Random\n");
    printf("2. User-defined\n");
    printf("Enter choice (1 or 2): ");
    
    if (scanf("%d", &generation_mode) != 1) {
        printf("Invalid choice\n");
        free_graph(graph);
        return;
    }
    
    if (generation_mode == 1) {
        double edge_density;
        printf("Enter edge density (0.0 to 1.0): ");
        if (scanf("%lf", &edge_density) != 1 || edge_density < 0.0 || edge_density > 1.0) {
            printf("Invalid edge density\n");
            free_graph(graph);
            return;
        }
        
        generate_random_graph(graph, edge_density);
    } else if (generation_mode == 2) {
        printf("Enter edges one by one (format: source destination, -1 -1 to finish):\n");
        int src, dest;
        while (1) {
            if (scanf("%d %d", &src, &dest) != 2) {
                printf("Invalid input\n");
                clear_input_buffer();
                continue;
            }
            
            if (src == -1 && dest == -1) break;
            
            if (src < 0 || dest < 0 || src >= num_vertices || dest >= num_vertices) {
                printf("Invalid vertex indices\n");
                continue;
            }
            
            add_edge(graph, src, dest);
        }
    } else {
        printf("Invalid choice\n");
        free_graph(graph);
        return;
    }
    
    print_graph(graph);
    
    printf("Enter filename to save the graph (or empty to skip): ");
    clear_input_buffer();
    if (fgets(filename, sizeof(filename), stdin) && filename[0] != '\n') {
        filename[strcspn(filename, "\n")] = 0;
        save_graph_to_file(graph, filename);
        printf("Graph saved to %s\n", filename);
    }
    
    free_graph(graph);
}

void chat_mode() {
    char input[MAX_INPUT];
    char response[MAX_RESPONSE];
    
    printf("\nChat Mode\n");
    printf("=========\n");
    printf("Type your request in natural language or 'exit' to quit\n");
    
    while (1) {
        printf("\n> ");
        if (!fgets(input, sizeof(input), stdin)) break;
        
        input[strcspn(input, "\n")] = 0;
        
        if (strcmp(input, "exit") == 0) break;
        
        if (strlen(input) < 5) {
            printf("Please provide a more detailed request\n");
            continue;
        }
        
        char prompt[MAX_INPUT + 100];
        snprintf(prompt, sizeof(prompt), 
                 "Generate parameters for a graph based on this request: \"%s\". "
                 "Please respond with just the required parameters in this format - "
                 "vertices: <number>, bidirectional: <true/false>, density: <0.0-1.0>", 
                 input);
        
        if (!query_llm(prompt, response, MAX_RESPONSE)) {
            printf("Failed to get response from LLM\n");
            continue;
        }
        
        int num_vertices;
        bool is_bidirectional;
        double edge_density;
        
        if (!parse_llm_response(response, &num_vertices, &is_bidirectional, &edge_density)) {
            printf("Failed to parse LLM response\n");
            continue;
        }
        
        printf("Creating graph with %d vertices, %s, edge density: %.2f\n", 
               num_vertices, 
               is_bidirectional ? "bidirectional" : "unidirectional", 
               edge_density);
        
        Graph* graph = create_graph(num_vertices, is_bidirectional);
        if (!graph) {
            printf("Failed to create graph\n");
            continue;
        }
        
        generate_random_graph(graph, edge_density);
        print_graph(graph);
        
        char filename[100];
        printf("Enter filename to save the graph (or empty to skip): ");
        if (fgets(filename, sizeof(filename), stdin) && filename[0] != '\n') {
            filename[strcspn(filename, "\n")] = 0;
            save_graph_to_file(graph, filename);
            printf("Graph saved to %s\n", filename);
        }
        
        free_graph(graph);
    }
}
