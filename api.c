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
        fprintf(stderr, "ERROR: Failed to initialize CURL\n");
        return;
    }

    char *url = "http://localhost:8080/v1/chat/completions";
    char post_data[8192];

    snprintf(post_data, sizeof(post_data),
             "{ \"model\": \"qwen2.5-7b-instruct-1m\", "
             "\"messages\": ["
             "{ \"role\": \"system\", \"content\": \"You are a graph generation assistant. Your task is to generate a valid random graph representation as a JSON object. Return ONLY JSON, no explanations or comments. Always include: { \\\"nodes\\\": <integer>, \\\"edges\\\": [[A,B], [C,D], ...] }. Ensure: (1) Node indexes start at 0, (2) No duplicate or self-loop edges, (3) The number of edges matches the request.\" },"
             "{ \"role\": \"user\", \"content\": \"%s\" }"
             "], \"max_tokens\": 100 }",
             userPrompt);

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, "Accept: application/json");

    curl_easy_setopt(curl, CURLOPT_URL, url); // Set url address for http request
    curl_easy_setopt(curl, CURLOPT_POST, 1L); // Set POST method instead of GET
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data); // Set post_data
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers); // Set HTTP headers
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, parseAndPrintResponse); // Set default output function (from stdout to parse function)

    res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        fprintf(stderr, "CURL ERROR: %s\n", curl_easy_strerror(res));
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
}
