#!/bin/bash

echo "Installing GraphCLI dependencies..."

if command -v apt-get &> /dev/null; then
    sudo apt-get update
    sudo apt-get install -y libcurl4-openssl-dev libcjson-dev graphviz
elif command -v dnf &> /dev/null; then
    sudo dnf install -y libcurl-devel json-c-devel graphviz
elif command -v brew &> /dev/null; then
    brew install curl cjson graphviz
else
    echo "Package manager not detected. Please install manually:"
    echo "- libcurl"
    echo "- cjson/json-c"
    echo "- graphviz (optional, for visualization)"
fi

echo "Dependencies installation complete"