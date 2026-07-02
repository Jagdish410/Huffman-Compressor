#include "Decoder.h"
#include "HuffmanTree.h"

#include <fstream>
#include <iostream>
#include <bitset>

bool Decoder::decompress(
    const std::string &inputFile,
    const std::string &outputFile)
{
    std::ifstream in(inputFile, std::ios::binary);

    if (!in)
    {
        return false;
    }

    // Read frequency table
    // Initialize frequency table
    int freq[256] = {0};

    // Read number of unique characters
    int uniqueCount;

    in.read(reinterpret_cast<char *>(&uniqueCount), sizeof(int));

    // Read only the used characters
    for (int i = 0; i < uniqueCount; i++)
    {
        unsigned char ch;
        int frequency;

        in.read(reinterpret_cast<char *>(&ch), sizeof(unsigned char));

        in.read(reinterpret_cast<char *>(&frequency), sizeof(int));

        freq[ch] = frequency;
    }

    // Read padding information
    unsigned char padding;

    in.read(reinterpret_cast<char *>(&padding), 1);

    // Rebuild Huffman Tree
    HuffmanTree tree;

    tree.buildTree(freq);

    // Read remaining compressed bytes
    std::string bitString;

    unsigned char byte;

    while (in.read(reinterpret_cast<char *>(&byte), 1))
    {
        std::bitset<8> bits(byte);

        bitString += bits.to_string();
    }

    // Remove padding bits
    if (padding > 0)
    {
        bitString.erase(bitString.size() - padding);
    }

    // Decode bit string
    std::ofstream out(outputFile, std::ios::binary);

    if (!out)
    {
        return false;
    }

    Node *current = tree.getRoot();

    for (char bit : bitString)
    {
        if (bit == '0')
        {
            current = current->left;
        }
        else
        {
            current = current->right;
        }

        // Leaf node reached
        if (current->left == nullptr && current->right == nullptr)
        {
            out.put(static_cast<char>(current->data));

            current = tree.getRoot();
        }
    }

    out.close();

    in.close();

    return true;
}
