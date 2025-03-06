#!/bin/bash

echo "Installing required dependencies..."
sudo apt-get update
sudo apt-get install -y libcurl4-openssl-dev libcjson-dev graphviz

echo "Creating directories..."
mkdir -p build/obj output

echo "Building GraphCLI..."
make clean
make

echo "Installation complete. Run ./graph_cli to start the program."
