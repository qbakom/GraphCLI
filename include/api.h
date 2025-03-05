#ifndef API_H
#define API_H

void sendQuery(char *userPrompt);
void parseUserPrompt(const char* prompt, int* vertices, int* edges, int* isDirected);
void createFallbackGraph(const char* userPrompt);

extern int use_fallback;

#endif
