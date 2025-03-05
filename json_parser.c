#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cjson/cJSON.h>
#include "include/json_parser.h"

int parsed_nodes = 0;
int parsed_edges[100][2];
int edge_count = 0;

static char response_buffer[65536] = {0};
static size_t buffer_pos = 0;

int parseAndPrintResponse(char *response, int size, int n) {
    size_t real_size = size * n;
    if (buffer_pos + real_size >= sizeof(response_buffer)) {
        fprintf(stderr, "[!] Response too large for buffer\n");
        return 0;
    }
    memcpy(response_buffer + buffer_pos, response, real_size);
    buffer_pos += real_size;
    response_buffer[buffer_pos] = '\0';
    return real_size;
}

void parseResponseBuffer() {
    parsed_nodes = 0;
    edge_count = 0;
    if (buffer_pos == 0) {
        printf("[!] Empty response received\n");
        return;
    }
    cJSON *root = cJSON_Parse(response_buffer);
    if (!root) {
        const char *error_ptr = cJSON_GetErrorPtr();
        printf("[!] JSON Parse Error: %s\n", error_ptr ? error_ptr : "Unknown error");
        printf("Response: %.100s...\n", response_buffer);
        return;
    }
    cJSON *choices = cJSON_GetObjectItem(root, "choices");
    if (!choices || !cJSON_IsArray(choices)) {
        printf("[!] No 'choices' array found in response\n");
        cJSON_Delete(root);
        return;
    }
    cJSON *choice = cJSON_GetArrayItem(choices, 0);
    if (!choice) {
        printf("[!] No items in 'choices' array\n");
        cJSON_Delete(root);
        return;
    }
    cJSON *message = cJSON_GetObjectItem(choice, "message");
    if (!message) {
        printf("[!] No 'message' found in response\n");
        cJSON_Delete(root);
        return;
    }
    cJSON *content = cJSON_GetObjectItem(message, "content");
    if (!content || !cJSON_IsString(content)) {
        printf("[!] No 'content' string found in response\n");
        cJSON_Delete(root);
        return;
    }
    const char *content_str = content->valuestring;
    printf("Raw response: %s\n\n", content_str);
    char *json_start = strchr(content_str, '{');
    char *json_end = strrchr(content_str, '}');
    if (!json_start || !json_end || json_end <= json_start) {
        printf("[!] Could not find valid JSON object in response\n");
        cJSON_Delete(root);
        return;
    }
    size_t json_len = json_end - json_start + 1;
    char *json_str = malloc(json_len + 1);
    if (!json_str) {
        printf("[!] Memory allocation failed\n");
        cJSON_Delete(root);
        return;
    }
    strncpy(json_str, json_start, json_len);
    json_str[json_len] = '\0';
    cJSON *graph_json = cJSON_Parse(json_str);
    if (!graph_json) {
        printf("[!] Failed to parse graph JSON: %s\n", json_str);
        free(json_str);
        cJSON_Delete(root);
        return;
    }
    cJSON *nodes = cJSON_GetObjectItem(graph_json, "nodes");
    if (!nodes || !cJSON_IsNumber(nodes)) {
        printf("[!] No valid 'nodes' field found\n");
        free(json_str);
        cJSON_Delete(graph_json);
        cJSON_Delete(root);
        return;
    }
    parsed_nodes = nodes->valueint;
    cJSON *edges = cJSON_GetObjectItem(graph_json, "edges");
    if (!edges || !cJSON_IsArray(edges)) {
        printf("[!] No valid 'edges' array found\n");
        free(json_str);
        cJSON_Delete(graph_json);
        cJSON_Delete(root);
        return;
    }
    int edge_array_size = cJSON_GetArraySize(edges);
    edge_count = 0;
    for (int i = 0; i < edge_array_size && i < 100; i++) {
        cJSON *edge = cJSON_GetArrayItem(edges, i);
        if (cJSON_IsArray(edge) && cJSON_GetArraySize(edge) == 2) {
            cJSON *src = cJSON_GetArrayItem(edge, 0);
            cJSON *dest = cJSON_GetArrayItem(edge, 1);
            if (cJSON_IsNumber(src) && cJSON_IsNumber(dest)) {
                parsed_edges[edge_count][0] = src->valueint;
                parsed_edges[edge_count][1] = dest->valueint;
                edge_count++;
            }
        }
    }
    printf("Successfully parsed graph with %d nodes and %d edges\n", parsed_nodes, edge_count);
    free(json_str);
    cJSON_Delete(graph_json);
    cJSON_Delete(root);
}

void finishJsonParsing() {
    parseResponseBuffer();
    buffer_pos = 0;
    response_buffer[0] = '\0';
}
