#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <ctype.h>
#include <time.h>
#include "include/api.h"
#include "include/json_parser.h"

// Flag to indicate if we need to use the fallback mechanism
int use_fallback = 0;

void sendQuery(char *userPrompt) {
    CURL *curl;
    CURLcode res;
    
    // Initialize CURL
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    
    if (!curl) {
        fprintf(stderr, "[!] CURL initialization failed\n");
        use_fallback = 1;
        return;
    }
    
    const char *url = "http://127.0.0.1:1234/v1/chat/completions";
    char post_data[4096];
    
    // Format the API request with the user's prompt
    snprintf(post_data, sizeof(post_data),
             "{ \"model\": \"local-model\", "
             "\"messages\": ["
             "{ \"role\": \"system\", \"content\": \"You are a graph generation assistant. Your task is to generate a valid graph representation as a JSON object. Return ONLY JSON, no explanations or comments. Always include: { \\\"nodes\\\": <integer>, \\\"edges\\\": [[A,B], [C,D], ...] }. Ensure: (1) Node indexes start at 0, (2) No duplicate or self-loop edges, (3) The number of edges is reasonable for the number of nodes.\" },"
             "{ \"role\": \"user\", \"content\": \"%s\" }"
             "], \"temperature\": 0.2, \"max_tokens\": 500, \"stream\": false }",
             userPrompt);

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, "Accept: application/json");

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, parseAndPrintResponse);
    
    // Adjust timeout settings - reduce timeouts since LM Studio seems to be hanging
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 20L);           // 20 second timeout
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 5L);     // 5 second connect timeout
    curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);      // Enable TCP keepalive

    printf("Sending request to LM Studio...\n");
    res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        fprintf(stderr, "[!] CURL error: %s\n", curl_easy_strerror(res));
        fprintf(stderr, "[!] Make sure LM Studio is running on http://127.0.0.1:1234\n");
        
        if (res == CURLE_OPERATION_TIMEDOUT) {
            fprintf(stderr, "[!] The request timed out. Trying fallback mechanism.\n");
            use_fallback = 1;
        } else if (res == CURLE_COULDNT_CONNECT) {
            fprintf(stderr, "[!] Could not connect to LM Studio. Using fallback mechanism.\n");
            use_fallback = 1;
        }
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    curl_global_cleanup();
}

// Parse the user prompt to extract basic graph parameters as a fallback
void parseUserPrompt(const char* prompt, int* vertices, int* edges, int* isDirected) {
    // Default values
    *vertices = 5;  // Default to 5 vertices
    *edges = 10;    // Default to 10 edges
    *isDirected = 0; // Default to undirected
    
    // Look for number of vertices
    const char* v_keywords[] = {"vertices", "vertex", "nodes", "node"};
    for (int i = 0; i < 4; i++) {
        char pattern[64];
        snprintf(pattern, sizeof(pattern), "%s", v_keywords[i]);
        
        const char* pos = strstr(prompt, pattern);
        if (pos) {
            // Look for a number before the keyword
            int j = 0;
            while (pos - j > prompt && j < 20) {
                j++;
                if (isdigit(*(pos-j))) {
                    int k = j;
                    while (pos - k > prompt && isdigit(*(pos-k-1))) {
                        k++;
                    }
                    char num_str[20] = {0};
                    strncpy(num_str, pos-k+1, k-1);
                    *vertices = atoi(num_str);
                    break;
                }
            }
        }
    }
    
    // Look for number of edges
    const char* e_keywords[] = {"edges", "edge", "connections", "links"};
    for (int i = 0; i < 4; i++) {
        char pattern[64];
        snprintf(pattern, sizeof(pattern), "%s", e_keywords[i]);
        
        const char* pos = strstr(prompt, pattern);
        if (pos) {
            // Look for a number before the keyword
            int j = 0;
            while (pos - j > prompt && j < 20) {
                j++;
                if (isdigit(*(pos-j))) {
                    int k = j;
                    while (pos - k > prompt && isdigit(*(pos-k-1))) {
                        k++;
                    }
                    char num_str[20] = {0};
                    strncpy(num_str, pos-k+1, k-1);
                    *edges = atoi(num_str);
                    break;
                }
            }
        }
    }
    
    // Check if directed is mentioned
    if (strstr(prompt, "directed") != NULL) {
        *isDirected = 1;
    }
    
    // Ensure values are reasonable
    if (*vertices <= 0) *vertices = 5;
    if (*edges <= 0) *edges = 10;
    if (*edges > (*vertices * (*vertices - 1)) / 2 && *isDirected == 0) {
        *edges = (*vertices * (*vertices - 1)) / 2;  // Max edges for undirected
    } else if (*edges > *vertices * (*vertices - 1) && *isDirected == 1) {
        *edges = *vertices * (*vertices - 1);  // Max edges for directed
    }
}

// Create a fallback graph when LM Studio fails
void createFallbackGraph(const char* userPrompt) {
    int vertices, edges, isDirected;
    
    printf("[!] Using fallback graph generation method\n");
    parseUserPrompt(userPrompt, &vertices, &edges, &isDirected);
    
    printf("[i] Creating a%s graph with %d vertices and %d edges\n", 
          isDirected ? " directed" : "n undirected", vertices, edges);
    
    // Set the global variables for graph creation
    parsed_nodes = vertices;
    edge_count = 0;
    
    // Generate random edges
    srand(time(NULL));
    
    while (edge_count < edges) {
        int src = rand() % vertices;
        int dest = rand() % vertices;
        
        if (src != dest) {  // Avoid self-loops
            // Check if edge already exists
            int duplicate = 0;
            for (int i = 0; i < edge_count; i++) {
                if (parsed_edges[i][0] == src && parsed_edges[i][1] == dest) {
                    duplicate = 1;
                    break;
                }
                // For undirected graphs, also check reverse edge
                if (!isDirected && parsed_edges[i][0] == dest && parsed_edges[i][1] == src) {
                    duplicate = 1;
                    break;
                }
            }
            
            if (!duplicate) {
                parsed_edges[edge_count][0] = src;
                parsed_edges[edge_count][1] = dest;
                edge_count++;
            }
        }
    }
    
    printf("[i] Successfully created fallback graph\n");
}
