#include <iostream>
#include "FileReader.h"
#include "HuffmanTree.h"

int main()
{
    FileReader reader;

    if (!reader.readFile("sample/sample.txt"))
    {
        std::cout << "Unable to open file.\n";
        return 1;
    }

    const int* freq = reader.getFrequency();

    HuffmanTree tree;

    tree.buildTree(freq);

    std::cout << "Huffman Tree:\n\n";

    tree.printTree();

    return 0;
}