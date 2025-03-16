#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <ctype.h>
#include <time.h>
#include "api.h"
#include "json_parser.h"

int use_fallback = 0;

void sendQuery(char *userPrompt) {
    CURL *curl;
    CURLcode res;
    
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    
    if (!curl) {
        fprintf(stderr, "[!] CURL initialization failed\n");
        use_fallback = 1;
        return;
    }
    
    const char *url = "http://127.0.0.1:1234/v1/chat/completions";
    char post_data[4096];
    
    snprintf(post_data, sizeof(post_data),
             "{ \"model\": \"local-model\", "
             "\"messages\": ["
             "{ \"role\": \"system\", \"content\": \"You are a graph generation assistant. Generate ONLY valid JSON with no other text. Format: {\\\"nodes\\\": <integer>, \\\"edges\\\": [[A,B], [C,D], ...]}. Rules: 1) Nodes are numbered from 0 to N-1. 2) No duplicate edges or self-loops. 3) IMPORTANT: Every node must be connected to at least one other node. 4) For undirected graphs, list each edge once - your code will handle bidirectionality. 5) Ensure exactly the requested number of edges.\" },"
             "{ \"role\": \"user\", \"content\": \"%s\" }"
             "], \"temperature\": 0.1, \"max_tokens\": 1000, \"stream\": false }",
             userPrompt);

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, "Accept: application/json");

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, parseAndPrintResponse);
    
    // curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L);
    // curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10L);
    // curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);

    res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        fprintf(stderr, "[!] CURL error: %s\n", curl_easy_strerror(res));
        fprintf(stderr, "[!] Make sure LM Studio is running on http://127.0.0.1:1234\n");
        use_fallback = 1;
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    curl_global_cleanup();
}
