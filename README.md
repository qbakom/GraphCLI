# Graph Generator

A console application in C for generating graphs with customizable vertices and edges.

## Features

- Create graphs with any number of vertices
- Support for both bidirectional and unidirectional graphs
- Two interaction modes:
  - Ordered Mode: Parameter-based graph generation
  - Chat Mode: Natural language processing using a local LLM server

## Dependencies

- libcurl (for HTTP communication with the LLM server)
- jansson (for JSON parsing)

## Building

```bash
make
```

## Usage

Run the program:

```bash
./graph_generator
```

### Ordered Mode

1. Enter the number of vertices
2. Choose if the graph is bidirectional
3. Select graph generation method:
   - Random: Set edge density between 0.0 and 1.0
   - User-defined: Add edges manually

### Chat Mode

Communicate with the program in natural language.
Examples:
- "Create a graph with 5 vertices"
- "Generate a random undirected graph with 10 nodes and moderate connectivity"

## LLM Integration

The Chat Mode requires a running LM Studio server at http://127.0.0.1:1234 with a deepseek model.
