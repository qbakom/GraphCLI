#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

int validateInput(int numVertices) {
    return numVertices > 0;
}

void handleError(const char* message) {
    fprintf(stderr, "Error: %s\n", message);
    exit(EXIT_FAILURE);
}
