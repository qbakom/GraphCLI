#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cjson/cJSON.h>
#include "include/json_parser.h"

int parseAndPrintResponse(char *response, int size, int n) {
    int totalSize = size * n;

    char *json = (char *)malloc(totalSize + 1);
    if (!json) {
        fprintf(stderr, "[!]: Memory allocation failure\n");
        return totalSize;
    }
    strncpy(json, (char *)response, totalSize);
    json[totalSize] = '\0';

    cJSON *root = cJSON_Parse(json);
    if (root == NULL) {
        fprintf(stderr, "[!]: JSON parsing error\n");
        free(json);
        return totalSize;
    }

    cJSON *choices = cJSON_GetObjectItem(root, "choices");
    if (choices && cJSON_IsArray(choices)) {
        cJSON *firstChoice = cJSON_GetArrayItem(choices, 0);
        if (firstChoice) {
            cJSON *text = cJSON_GetObjectItem(firstChoice, "text");
            if (text && cJSON_IsString(text)) {
                printf("Chatbot: %s\n", text->valuestring);
            } else {
                fprintf(stderr, "[!] No matching text in JSON response. Try again!\n");
            }
        }
    }
    cJSON_Delete(root);
    free(json);

    return totalSize;
}
