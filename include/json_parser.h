#ifndef JSON_PARSER_H
#define JSON_PARSER_H

/**
 * Parse JSON response from LM Studio API
 * Extracts nodes and edges into global variables
 */
int parseAndPrintResponse(char *response, int size, int n);

extern int parsed_nodes;
extern int parsed_edges[100][2];
extern int edge_count;

#endif
