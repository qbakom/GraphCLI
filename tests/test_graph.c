#include <stdio.h>
#include <assert.h>
#include "graph.h"

#define TEST_SUCCESS "\033[1;32m[SUCCESS]\033[0m"
#define TEST_FAILURE "\033[1;31m[FAIL]\033[0m"

void test_result(int condition, const char *message) {
    if (condition) {
        printf("%s %s\n", TEST_SUCCESS, message);
    } else {
        printf("%s %s\n", TEST_FAILURE, message);
    }
}

// Graph creation testing
void test_graph_creation() {
    Graph *g = graph_create(5, 0);
    test_result(g != NULL, "Graph creation");
    test_result(g->V == 5, "Vertex count = 5");
    test_result(g->isDirected == 0, "Graph is undirected");
    graph_free(g);
}

// Adding edges testing
void test_add_edge() {
    Graph *g = graph_create(5, 0);
    test_result(graph_add_edge(g, 0, 1) == GRAPH_OK, "Adding edge 0 -> 1");
    test_result(graph_has_edge(g, 0, 1) == GRAPH_OK, "Checking edge existence 0 -> 1");
    test_result(graph_add_edge(g, 0, 1) == GRAPH_ERROR_EDGE_EXISTS, "Adding the same edge (should fail)");
    test_result(graph_add_edge(g, 5, 6) == GRAPH_ERROR_INVALID_VERTEX, "Adding an edge with invalid indices");
    graph_free(g);
}

// Directed graph testing
void test_directed_graph() {
    Graph *g = graph_create(4, 1);
    test_result(g != NULL, "Directed graph creation");
    test_result(graph_add_edge(g, 0, 1) == GRAPH_OK, "Adding directed edge 0 -> 1");
    test_result(graph_has_edge(g, 0, 1) == GRAPH_OK, "Checking directed edge 0 -> 1");
    test_result(graph_has_edge(g, 1, 0) == GRAPH_ERROR_EDGE_EXISTS, "Edge 1 -> 0 should not exist (directed graph)");
    graph_free(g);
}

// Random graph generation testing
void test_random_graph() {
    Graph *g = graph_generate_random(6, 10, 0);
    test_result(g != NULL, "Random graph generation");
    test_result(g->V == 6, "Random graph has 6 vertices");
    test_result(g->degrees != NULL, "Degree array exists");
    graph_free(g);
}

int main() {
    printf("=== RUNNING TESTS ===\n");
    test_graph_creation();
    test_add_edge();
    test_directed_graph();
    test_random_graph();
    printf("=== ALL TESTS COMPLETED ===\n");
    return 0;
}
