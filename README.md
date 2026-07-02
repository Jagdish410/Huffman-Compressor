# Huffman-Compressor
A C++ command-line tool for compressing and decompressing files using the Huffman Coding algorithm.

# Huffman File Compressor

A lossless file compression and decompression tool written in C++ using the Huffman Coding algorithm.

## Features

- Compress any file using Huffman Coding
- Decompress files back to their original form
- Command-line interface
- Binary file handling
- Automatic Huffman tree construction
- Variable-length binary encoding
- Lossless compression

## Project Structure

```
Huffman-Compressor/
│
├── src/
├── sample/
├── output/
├── README.md
└── .gitignore
```

## Build

```bash
g++ -std=c++17 src/*.cpp -o huffman
```

## Usage

Compress

```bash
./huffman compress input.txt output.huff
```

Decompress

```bash
./huffman decompress output.huff recovered.txt
```

## Algorithm

1. Read the input file.
2. Count the frequency of every byte.
3. Build the Huffman Tree using a priority queue.
4. Generate Huffman codes.
5. Encode the file into a binary bitstream.
6. Store the compressed data and metadata.
7. Reconstruct the Huffman tree during decompression.
8. Recover the original file exactly.

## Technologies

- C++
- STL
- Priority Queue
- Binary File I/O
- Object-Oriented Programming

## Future Improvements

- Store Huffman tree instead of frequencies.
- Reduce metadata size.
- Better compression for small files.
- GUI / Web interface.
- Canonical Huffman Coding.

## Author

Jagdish