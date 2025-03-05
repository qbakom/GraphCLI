#include "llm_client.h"
#include <ctype.h>
#include <jansson.h>

static size_t write_memory_callback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t realsize = size * nmemb;
    MemoryStruct* mem = (MemoryStruct*)userp;
    
    char* ptr = realloc(mem->memory, mem->size + realsize + 1);
    if (!ptr) return 0;
    
    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;
    
    return realsize;
}

bool query_llm(const char* prompt, char* response, size_t response_size) {
    CURL* curl;
    CURLcode res;
    struct curl_slist* headers = NULL;
    MemoryStruct chunk;
    
    chunk.memory = malloc(1);
    chunk.size = 0;
    
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    
    if (curl) {
        char json_data[4096];
        snprintf(json_data, sizeof(json_data), 
                 "{\"model\":\"deepseek-chat\",\"messages\":[{\"role\":\"user\",\"content\":\"%s\"}],\"stream\":false}",
                 prompt);
        
        headers = curl_slist_append(headers, "Content-Type: application/json");
        
        curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:1234/v1/chat/completions");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_memory_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&chunk);
        
        res = curl_easy_perform(curl);
        
        if (res != CURLE_OK) {
            snprintf(response, response_size, "Failed to connect to LLM server: %s", curl_easy_strerror(res));
            free(chunk.memory);
            curl_slist_free_all(headers);
            curl_easy_cleanup(curl);
            curl_global_cleanup();
            return false;
        }
        
        json_error_t error;
        json_t* root = json_loads(chunk.memory, 0, &error);
        
        if (!root) {
            snprintf(response, response_size, "Error parsing JSON response: %s", error.text);
            free(chunk.memory);
            curl_slist_free_all(headers);
            curl_easy_cleanup(curl);
            curl_global_cleanup();
            return false;
        }
        
        json_t* choices = json_object_get(root, "choices");
        if (json_array_size(choices) > 0) {
            json_t* first_choice = json_array_get(choices, 0);
            json_t* message = json_object_get(first_choice, "message");
            json_t* content = json_object_get(message, "content");
            
            const char* content_str = json_string_value(content);
            if (content_str) {
                strncpy(response, content_str, response_size - 1);
                response[response_size - 1] = '\0';
            }
        }
        
        json_decref(root);
        free(chunk.memory);
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }
    
    curl_global_cleanup();
    return true;
}

bool parse_llm_response(const char* response, int* num_vertices, bool* is_bidirectional, double* edge_density) {
    if (!response || !num_vertices || !is_bidirectional || !edge_density) return false;
    
    *num_vertices = 0;
    *is_bidirectional = true;
    *edge_density = 0.3;
    
    char* vertices_str = strstr(response, "vertices:");
    char* bidirectional_str = strstr(response, "bidirectional:");
    char* density_str = strstr(response, "density:");
    
    if (vertices_str) {
        vertices_str += strlen("vertices:");
        while (*vertices_str && !isdigit(*vertices_str)) vertices_str++;
        if (*vertices_str) *num_vertices = atoi(vertices_str);
    }
    
    if (bidirectional_str) {
        bidirectional_str += strlen("bidirectional:");
        while (*bidirectional_str && !isalpha(*bidirectional_str)) bidirectional_str++;
        if (strncasecmp(bidirectional_str, "false", 5) == 0 ||
            strncasecmp(bidirectional_str, "no", 2) == 0 ||
            strncasecmp(bidirectional_str, "0", 1) == 0) {
            *is_bidirectional = false;
        }
    }
    
    if (density_str) {
        density_str += strlen("density:");
        while (*density_str && !isdigit(*density_str) && *density_str != '.') density_str++;
        if (*density_str) *edge_density = atof(density_str);
    }
    
    return *num_vertices > 0;
}
