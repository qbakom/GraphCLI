# GraphCLI

A command-line interface for generating and visualizing graphs, with LLM integration through LM Studio.

## Features
- **Graph Creation:** Supports both manual and AI-generated graphs.
- **Graph Representation:** Uses an adjacency list structure.
- **Two Interaction Modes:**
  - **Manual Mode:** User-defined graph parameters.
  - **Chat Mode:** AI-assisted graph generation via LM Studio.
- **Graph Storage:** Save graphs to files for later use.
- **Error Handling:** Input validation to prevent incorrect graph structures.

---

## Project Structure
```
GraphCLI/
│── include/          # Header files
│   ├── graph.h       # Graph data structures and functions
│   ├── api.h         # API interaction functions
│   └── json_parser.h # JSON parsing functions
│── src/              # Source files
│   ├── main.c        # Main program logic
│   ├── graph.c       # Graph implementation
│   ├── api.c         # Handles API requests for AI-generated graphs
│   └── json_parser.c # Parses JSON responses from AI
│── test/             # Tests
│   └── test_graph.c  # Graph-related tests
│── Makefile          # Build system configuration
│── README.md         # Project documentation
│── install.sh        # Installation script (if used)
```

---

## Requirements

- GCC compiler
- `libcurl4-openssl-dev` (for HTTP requests)
- `libcjson-dev` (for JSON parsing)
- **LM Studio** (running locally on port `1234` for AI graph generation)

---

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

---

## Usage

### Running the Application
```bash
./graph_cli
```

### Operation Modes
1. **Manual Mode:** Create graphs by specifying vertices and edges manually or generate random graphs.
2. **Chat Mode:** Use natural language to describe a graph and generate it with LM Studio's AI.

---

## Examples

- Creating a complete graph: "Create a complete graph with 5 vertices"
- Creating a star graph: "Generate a star graph with 8 vertices"
- Creating a path graph: "Build a path graph with 10 vertices"
---

## Tests
To test the application, use:
```bash
make test
./test_graph
```
Example output:
```
=== RUNNING TESTS ===
[SUCCESS] Graph creation
[SUCCESS] Vertex count = 5
[SUCCESS] Graph is undirected
[SUCCESS] Adding edge 0 -> 1
[SUCCESS] Checking edge existence 0 -> 1
[SUCCESS] Random graph generation
[SUCCESS] Graph successfully saved to file
=== ALL TESTS COMPLETED ===
```

---

## Saving Graphs to a File
After generating a graph, the program prompts:
```
Do you want to save the graph to a file? (yes/no)
```
If you choose **yes**, enter a filename:
```
Enter filename: my_graph.txt
```
Example output in `my_graph.txt`:
```
5 0

0 1
1 2
2 3
3 4
4 0
```
The first line represents **number of vertices** and **graph type (0 = undirected, 1 = directed)**.  
Subsequent lines represent **edges**.

---

## Troubleshooting

### **1. LM Studio Is Not Running**
If AI-generated graphs fail, ensure LM Studio is running on the proper port:
```bash
netstat -an | grep 1234
```
If it's not running, start it manually.

---

