#ifndef LLM_CLIENT_H
#define LLM_CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <curl/curl.h>

typedef struct {
    char* memory;
    size_t size;
} MemoryStruct;

bool query_llm(const char* prompt, char* response, size_t response_size);
bool parse_llm_response(const char* response, int* num_vertices, bool* is_bidirectional, double* edge_density);

#endif
