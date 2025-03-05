#ifndef JSON_PARSER_H
#define JSON_PARSER_H

/**
 * Parse the JSON response from the LLM API and extract graph data
 * 
 * @param response The JSON response from the API
 * @param size Size of each chunk
 * @param n Number of chunks
 * @return Total size of processed data
 */
int parseAndPrintResponse(char *response, int size, int n);

extern int parsed_nodes;
extern int parsed_edges[100][2];
extern int edge_count;

#endif /* JSON_PARSER_H */
