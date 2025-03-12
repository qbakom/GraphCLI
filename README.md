# GraphCLI Documentation

A command-line interface for generating and visualizing graphs, with LLM integration through LM Studio.

## Introduction
Graphs are mathematical structures used to model relationships between objects. A **graph** consists of **vertices** (nodes) connected by **edges** (connections). There are two main types of graphs:

- **Directed Graphs:** Edges have a direction, meaning they go from one vertex to another.  
- **Undirected Graphs:** Edges are bidirectional, meaning they connect vertices in both directions.

GraphCLI is a powerful command-line tool designed for creating, visualizing, and managing graphs. It supports both manual graph creation and AI-assisted graph generation through integration with LM Studio. This tool can be used as a foundation for larger projects such as:

- **Pathfinding Algorithms** (Dijkstra, A*)  
- **Network Simulations** (Social networks, traffic routing)  
- **Dependency Graphs** (Task scheduling, compiler dependencies)  
- **AI Graph-based Reasoning**

This documentation will guide you through installing, using, and troubleshooting the application.

---

## Features
- **Graph Creation:** Supports both manual and AI-generated graphs.  
- **Graph Representation:** Uses an adjacency list structure. In simple terms, each vertex has a list of other vertices (neighbors) that it connects to by outgoing edges.  
- **Two Interaction Modes:**  
  1. **Manual Mode:** Users define graph parameters step by step through interactive prompts.  
  2. **AI Mode:** Uses LM Studio to generate graphs based on user-defined descriptions.  
- **Graph Storage:** Save and load graphs from files.  
- **Error Handling:** Built-in validation to prevent incorrect graph structures.

> **Recommended Language Model:** This program was tested with and is optimized for [`qwen2.5-7b-instruct-1m`](https://lmstudio.ai/model/qwen2.5-7b-1m). Using this model in LM Studio is highly recommended for the best results.

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
│── build/            # Build directory (created during compilation)
│── Makefile          # Build system configuration
│── README.md         # Project documentation
│── install.sh        # Installation script (if used)
```

---

## Installation and Compilation

### 1. Automated Installation (using `install.sh`)
If you prefer an automated approach, you can use the provided `install.sh` script located in the project’s root directory. This script typically performs the following actions:

1. Checks and installs necessary dependencies (like `gcc` and `make`) if needed.  
2. Runs `make` to build the project.  
3. Installs the resulting executable (e.g., `graph_cli`) system-wide or in a specified directory.  

To run the script:
```bash
chmod +x install.sh
./install.sh
```

### 2. Manual Installation
1. **Dependencies:** You will need a C compiler (e.g., `gcc`) and `make` installed on your system.  
2. **Clone the Repository:**  
   ```bash
   git clone https://example.com/GraphCLI.git
   cd GraphCLI
   ```
3. **Build the Project:**  
   ```bash
   make
   ```
   After a successful build, an executable named `graph_cli` will be generated in the project’s root directory (or inside `build/`, depending on your Makefile configuration).  
4. **Run the Program:**  
   ```bash
   ./graph_cli
   ```

---

## LM Studio Setup
GraphCLI uses LM Studio for AI-based graph generation.

- **Download and Install LM Studio:** Refer to [LM Studio’s official documentation](https://lmstudio.ai/docs/app/basics) for the installation steps.
- **Start LM Studio:** By default, GraphCLI expects LM Studio on port **1234**. You can verify if LM Studio is running by checking:

  ```bash
  netstat -an | grep 1234
  ```

  Make sure a valid language model (e.g., `qwen2.5-7b-instruct-1m`) is loaded in LM Studio.
- **Configure API Settings:** If you need a custom port or specific headers, edit the relevant sections in `api.c`.

---

## Example Graph Creation

### Manual Mode Example
Below is a step-by-step example of creating a **directed** graph with **5 vertices** and **5 edges** in manual mode:

#### 1. Run the program:
```bash
./graph_cli
```

#### 2. Choose manual mode:
```bash
Choose program operation mode:
1. Manual parameters input
2. Chat mode with LM Studio
Enter choice (1 or 2): 1
```

#### 3. Define the number of vertices:
```bash
Enter the number of vertices: 5
```

#### 4. Select the graph type:
```bash
Graph type:
1. Directed (one-way edges)
2. Undirected (two-way edges)
Enter choice (1 or 2): 1
```

#### 5. Choose how to define edges:
```bash
Graph generation:
1. Manual edge input
2. Random graph
Enter choice (1 or 2): 1
```

#### 6. Specify the number of edges and enter them manually:
```bash
Enter the number of edges: 5

Input format for edges: <source> <destination>
Both source and destination must be integers between 0 and 4
Example: '0 1' creates an edge from vertex 0 to vertex 1

Enter edge 1 of 5 [source destination]: 1 2
Enter edge 2 of 5 [source destination]: 1 0
Enter edge 3 of 5 [source destination]: 2 1
Enter edge 4 of 5 [source destination]: 2 4
Enter edge 5 of 5 [source destination]: 3 2
```

#### 7. View the generated graph:
```bash
Graph with 5 vertices (directed):
Adjacency List:
0 (0 edges): NULL
1 (2 edges): 0 -> 2 -> NULL
2 (2 edges): 4 -> 1 -> NULL
3 (1 edges): 2 -> NULL
4 (0 edges): NULL
```

#### 8. Save the graph:
```bash
Do you want to save the graph to a file? (yes/no): yes
Enter filename (without extension): test
[i] Graph saved to test
[i] Graph image generated: test.png
```

### AI Mode Example
Below is a step-by-step example of using **AI mode** with LM Studio to generate a graph:

#### 1. Run the program:
```bash
./graph_cli
```

#### 2. Choose AI mode:
```bash
Choose program operation mode:
1. Manual parameters input
2. Chat mode with LM Studio
Enter choice (1 or 2): 2
```

#### 3. Describe the graph you want to generate:
```bash
=== Chat Mode ===
Describe the graph you want to create in natural language.
Example: 'Create a directed graph with 5 vertices and 7 edges'

Your request: Create a directed graph with 6 vertices and 8 edges
```

#### 4. Program sends the request to LM Studio and displays the raw response:
```bash
Sending request to LM Studio...
Raw response: {
  "nodes": 6,
  "edges": [
      [0, 1],
      [1, 2],
      [2, 3],
      [3, 4],
      [4, 5],
      [5, 0],
      [2, 5],
      [3, 1]
  ]
}
```

#### 5. The generated graph is displayed:
```bash
Successfully parsed graph with 6 nodes and 8 edges
Creating graph with 6 vertices and 8 edges

Graph with 6 vertices (directed):
Adjacency List:
0 (1 edges): 1 -> NULL
1 (2 edges): 2 -> 3 -> NULL
2 (2 edges): 5 -> 3 -> NULL
3 (2 edges): 4 -> 1 -> NULL
4 (1 edges): 5 -> NULL
5 (2 edges): 0 -> 2 -> NULL
```

#### 6. Save the AI-generated graph:
```bash
Do you want to save the graph to a file? (yes/no): yes
Enter filename (without extension): ai_graph
[i] Graph saved to ai_graph
[i] Graph image generated: ai_graph.png
```

---

## Troubleshooting

### LM Studio Is Not Running
If AI-generated graphs fail, ensure LM Studio is running on the correct port (`1234` by default):
```bash
netstat -an | grep 1234
```
If it's not running, start it manually following LM Studio’s documentation.

### Error Generating Graph Image
If you encounter the following error message:
```bash
[!] Error generating graph image
```
This likely indicates a problem with the Graphviz library, which is required for rendering graph images.

#### Possible Solutions:
1. **Ensure Graphviz is installed:**
   - On Ubuntu/Debian:
     ```bash
     sudo apt install graphviz
     ```
   - On macOS (using Homebrew):
     ```bash
     brew install graphviz
     ```
   - On Windows (using Chocolatey):
     ```powershell
     choco install graphviz
     ```

2. **Verify that Graphviz is accessible in the system path:**
   ```bash
   dot -V
   ```
   If this command does not return the Graphviz version, you may need to add its installation directory to your system’s PATH variable.

3. **Restart your terminal or system:** After installing Graphviz, restarting the terminal or system can help apply changes.

If the issue persists, refer to the official [Graphviz documentation](https://graphviz.org/) for further troubleshooting.

