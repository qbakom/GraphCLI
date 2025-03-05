#ifndef JSON_PARSER_H
#define JSON_PARSER_H

extern int parsed_nodes;
extern int edge_count;
extern int parsed_edges[100][2];

int parseAndPrintResponse(char *response, int size, int n);

#endif
