#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cjson/cJSON.h>
#include "include/json_parser.h"

int parsed_nodes = 0;
int parsed_edges[100][2]; 
int edge_count = 0;

int parseAndPrintResponse(char *response, int size, int n) {
    int totalSize = size * n;
    parsed_nodes = 0;
    edge_count = 0;

    // Allocate memory for the JSON response
    char *json = (char *)malloc(totalSize + 1);
    if (!json) {
        fprintf(stderr, "[!] Memory allocation failure\n");
        return totalSize;
    }
    strncpy(json, response, totalSize);
    json[totalSize] = '\0';

    printf("\nProcessing LM Studio response...\n");
    
    // Parse the main response JSON
    cJSON *root = cJSON_Parse(json);
    if (!root) {
        fprintf(stderr, "[!] JSON parsing error: %s\n", cJSON_GetErrorPtr());
        free(json);
        return totalSize;
    }

    // Extract choices array
    cJSON *choices = cJSON_GetObjectItem(root, "choices");
    if (!choices || !cJSON_IsArray(choices)) {
        fprintf(stderr, "[!] No 'choices' array found in response\n");
        cJSON_Delete(root);
        free(json);
        return totalSize;
    }

    // Get first choice
    cJSON *firstChoice = cJSON_GetArrayItem(choices, 0);
    if (!firstChoice) {
        fprintf(stderr, "[!] Empty 'choices' array\n");
        cJSON_Delete(root);
        free(json);
        return totalSize;
    }

    // Extract message
    cJSON *message = cJSON_GetObjectItem(firstChoice, "message");
    if (!message) {
        // Try 'text' field for some LLM API versions
        message = cJSON_GetObjectItem(firstChoice, "text");
        if (!message) {
            fprintf(stderr, "[!] No 'message' or 'text' field in response\n");
            cJSON_Delete(root);
            free(json);
            return totalSize;
        }
    }

    // Get content
    cJSON *content = NULL;
    if (cJSON_IsString(message)) {
        // Some APIs return text directly
        content = message;
    } else {
        // Standard format with message object
        content = cJSON_GetObjectItem(message, "content");
    }

    if (!content || !cJSON_IsString(content)) {
        fprintf(stderr, "[!] No 'content' field or not a string\n");
        cJSON_Delete(root);
        free(json);
        return totalSize;
    }

    const char *contentStr = content->valuestring;
    printf("Response from LM Studio:\n%s\n", contentStr);

    // Find JSON content within the response - sometimes it's surrounded by markdown or text
    char *jsonStart = strstr(contentStr, "{");
    char *jsonEnd = strrchr(contentStr, '}');
    
    if (!jsonStart || !jsonEnd || jsonEnd <= jsonStart) {
        fprintf(stderr, "[!] Could not find valid JSON in response\n");
        cJSON_Delete(root);
        free(json);
        return totalSize;
    }
    
    // Extract just the JSON part
    size_t jsonLen = jsonEnd - jsonStart + 1;
    char *jsonPart = (char *)malloc(jsonLen + 1);
    if (!jsonPart) {
        fprintf(stderr, "[!] Memory allocation failure for JSON extraction\n");
        cJSON_Delete(root);
        free(json);
        return totalSize;
    }
    
    strncpy(jsonPart, jsonStart, jsonLen);
    jsonPart[jsonLen] = '\0';
    
    // Parse the graph data
    cJSON *graphData = cJSON_Parse(jsonPart);
    free(jsonPart); // We no longer need this
    
    if (!graphData) {
        fprintf(stderr, "[!] Failed to parse graph data JSON: %s\n", cJSON_GetErrorPtr());
        cJSON_Delete(root);
        free(json);
        return totalSize;
    }

    // Extract nodes count
    cJSON *nodes = cJSON_GetObjectItem(graphData, "nodes");
    if (nodes && cJSON_IsNumber(nodes)) {
        parsed_nodes = nodes->valueint;
        if (parsed_nodes <= 0 || parsed_nodes > 100) {
            fprintf(stderr, "[!] Invalid node count: %d (must be 1-100)\n", parsed_nodes);
            if (parsed_nodes > 100) parsed_nodes = 100;
            else parsed_nodes = 0;
        }
    } else {
        fprintf(stderr, "[!] Missing or invalid 'nodes' field\n");
    }

    // Extract edges
    cJSON *edges = cJSON_GetObjectItem(graphData, "edges");
    if (edges && cJSON_IsArray(edges)) {
        edge_count = cJSON_GetArraySize(edges);
        if (edge_count > 100) {
            fprintf(stderr, "[!] Too many edges (%d), truncating to 100\n", edge_count);
            edge_count = 100;
        }
        
        for (int i = 0; i < edge_count; i++) {
            cJSON *edge = cJSON_GetArrayItem(edges, i);
            if (edge && cJSON_IsArray(edge) && cJSON_GetArraySize(edge) >= 2) {
                cJSON *src = cJSON_GetArrayItem(edge, 0);
                cJSON *dest = cJSON_GetArrayItem(edge, 1);
                
                if (src && dest && cJSON_IsNumber(src) && cJSON_IsNumber(dest)) {
                    parsed_edges[i][0] = src->valueint;
                    parsed_edges[i][1] = dest->valueint;
                } else {
                    fprintf(stderr, "[!] Edge %d: Invalid source or destination\n", i);
                    parsed_edges[i][0] = 0;
                    parsed_edges[i][1] = 1; // Default to edge 0->1
                }
            } else {
                fprintf(stderr, "[!] Invalid edge format at index %d\n", i);
                parsed_edges[i][0] = 0;
                parsed_edges[i][1] = 1; // Default to edge 0->1
            }
        }
    } else {
        fprintf(stderr, "[!] Missing or invalid 'edges' field\n");
    }

    cJSON_Delete(graphData);
    cJSON_Delete(root);
    free(json);

    printf("\nParsing complete - Extracted %d nodes and %d edges\n", parsed_nodes, edge_count);
    
    return totalSize;
}
