#ifndef JSON_PARSER_H
#define JSON_PARSER_H

int parseAndPrintResponse(char *response, int size, int n);
void finishJsonParsing();

extern int parsed_nodes;
extern int parsed_edges[100][2];
extern int edge_count;

#endif
