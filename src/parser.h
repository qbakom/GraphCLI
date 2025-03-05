#ifndef PARSER_H
#define PARSER_H

#include "graph.h"

int parseCommand(const char* command, Graph* graph);
int extractVertices(const char* command, int* vertices, int maxVertices);

#endif
