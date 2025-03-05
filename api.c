#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "include/api.h"
#include "include/json_parser.h"

void sendQuery(char *userPrompt) {
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "[!] Failed to initialize CURL\n");
        return;
    }

    char *url = "http://127.0.0.1:1234/v1/chat/completions";
    char post_data[8192];

    snprintf(post_data, sizeof(post_data),
             "{ \"model\": \"deepseek\", "
             "\"messages\": ["
             "{ \"role\": \"system\", \"content\": \"You are a graph generation assistant. Your task is to generate a valid graph representation as a JSON object. Return ONLY JSON, no explanations or comments. Always include: { \\\"nodes\\\": <integer>, \\\"edges\\\": [[A,B], [C,D], ...] }. Ensure: (1) Node indexes start at 0, (2) No duplicate or self-loop edges, (3) The number of edges is reasonable for the number of nodes.\" },"
             "{ \"role\": \"user\", \"content\": \"%s\" }"
             "], \"temperature\": 0.2, \"max_tokens\": 500 }",
             userPrompt);

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, "Accept: application/json");

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, parseAndPrintResponse);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L); // 30 second timeout

    printf("Sending request to LM Studio...\n");
    res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        fprintf(stderr, "[!] CURL error: %s\n", curl_easy_strerror(res));
        fprintf(stderr, "[!] Make sure LM Studio is running on http://127.0.0.1:1234\n");
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
}
