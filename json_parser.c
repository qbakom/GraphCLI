#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cjson/cJSON.h>
#include "include/api.h"
#include "include/json_parser.h"

int parsed_nodes = 0;
int parsed_edges[100][2]; 
int edge_count = 0;

int parseAndPrintResponse(char *response, int size, int n) {
    int totalSize = size * n;

    char *json = (char *)malloc(totalSize + 1);
    if (!json) {
        fprintf(stderr, "[!]: Memory allocation failure\n");
        return totalSize;
    }
    strncpy(json, response, totalSize);
    json[totalSize] = '\0';

    cJSON *root = cJSON_Parse(json);
    if (!root) {
        fprintf(stderr, "[!]: JSON parsing error.\n");
        free(json);
        return totalSize;
    }

    cJSON *choices = cJSON_GetObjectItem(root, "choices");
    if (!choices || !cJSON_IsArray(choices)) {
        fprintf(stderr, "[!] No 'choices' field in JSON response.\n");
        cJSON_Delete(root);
        free(json);
        return totalSize;
    }

    cJSON *firstChoice = cJSON_GetArrayItem(choices, 0);
    if (!firstChoice) {
        fprintf(stderr, "[!] Empty 'choices' array.\n");
        cJSON_Delete(root);
        free(json);
        return totalSize;
    }

    cJSON *message = cJSON_GetObjectItem(firstChoice, "message");
    if (!message) {
        fprintf(stderr, "[!] No 'message' field in JSON response.\n");
        cJSON_Delete(root);
        free(json);
        return totalSize;
    }

    cJSON *content = cJSON_GetObjectItem(message, "content");
    if (!content || !cJSON_IsString(content)) {
        fprintf(stderr, "[!] No 'content' field or not a string.\n");
        cJSON_Delete(root);
        free(json);
        return totalSize;
    }

    cJSON *graphData = cJSON_Parse(content->valuestring);
    if (!graphData) {
        fprintf(stderr, "[!]: Failed to parse 'content' JSON.\n");
        cJSON_Delete(root);
        free(json);
        return totalSize;
    }

    cJSON *nodes = cJSON_GetObjectItem(graphData, "nodes");
    if (nodes && cJSON_IsNumber(nodes)) {
        parsed_nodes = nodes->valueint;
    } else {
        fprintf(stderr, "[!] Missing 'nodes' field.\n");
    }

    cJSON *edges = cJSON_GetObjectItem(graphData, "edges");
    if (edges && cJSON_IsArray(edges)) {
        edge_count = cJSON_GetArraySize(edges);
        for (int i = 0; i < edge_count; i++) {
            cJSON *edge = cJSON_GetArrayItem(edges, i);
            if (edge && cJSON_IsArray(edge) && cJSON_GetArraySize(edge) == 2) {
                parsed_edges[i][0] = cJSON_GetArrayItem(edge, 0)->valueint;
                parsed_edges[i][1] = cJSON_GetArrayItem(edge, 1)->valueint;
            }
        }
    } else {
        fprintf(stderr, "[!] Missing or invalid 'edges' field.\n");
    }

    cJSON_Delete(graphData);
    cJSON_Delete(root);
    free(json);


    printf("\n");
    printf("Generated graph parameters: \n");
    printf("Nodes: %d\n", parsed_nodes);
    for (int i = 0; i < edge_count; i++) {
        printf("Edge %d: [ %d | %d ]\n", i, parsed_edges[i][0], parsed_edges[i][1]);
    }

    return totalSize;
}
