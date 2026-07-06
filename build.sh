#!/usr/bin/env bash
set -e

pip install -r requirements.txt

g++ -std=c++17 src/*.cpp -o huffman