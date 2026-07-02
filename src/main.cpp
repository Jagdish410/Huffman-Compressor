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

    tree.generateCodes();

    const std::unordered_map<unsigned char, std::string>& codes = tree.getCodes();

    std::cout << "Huffman Codes:\n\n";

    for (const auto& pair : codes)
    {
        std::cout << pair.first << " : " << pair.second << std::endl;
    }

    return 0;
}