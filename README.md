# GraphCLI

A command-line interface for generating and visualizing graphs, with LLM integration through LM Studio.

## Requirements

- GCC compiler
- libcurl4-openssl-dev
- libcjson-dev
- LM Studio (running locally on port 1234)

## Installation

### Automatic Installation

```bash
chmod +x install.sh
./install.sh
```

### Manual Installation

1. Install dependencies:
```bash
sudo apt-get update
sudo apt-get install -y libcurl4-openssl-dev libcjson-dev
```

2. Build the application:
```bash
make
```

## Usage

Run the application:
```bash
./graph_cli
```

### Operation Modes

1. **Manual Mode**: Create graphs by specifying vertices and edges manually or generate random graphs
2. **Chat Mode**: Use natural language to describe a graph and generate it with LM Studio's AI

### Examples

- Creating a complete graph: "Create a complete graph with 5 vertices"
- Creating a star graph: "Generate a star graph with 8 vertices"
- Creating a path graph: "Build a path graph with 10 vertices"
