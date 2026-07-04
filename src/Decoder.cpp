#include "Decoder.h"
#include "HuffmanTree.h"
#include <fstream>
#include <iostream>

using std::string;
using std::ifstream;
using std::ofstream;
using std::ios;

bool Decoder::decompress(
    const string &inputFile,
    const string &outputFile)
{
    ifstream in(inputFile, ios::binary);
    if (!in)
    {
        return false;
    }

    // 1. Read frequency table metadata
    int freq[256] = {0};
    int uniqueCount;
    in.read(reinterpret_cast<char *>(&uniqueCount), sizeof(int));

    for (int i = 0; i < uniqueCount; i++)
    {
        unsigned char ch;
        int frequency;

        in.read(reinterpret_cast<char *>(&ch), sizeof(unsigned char));
        in.read(reinterpret_cast<char *>(&frequency), sizeof(int));

        freq[ch] = frequency;
    }

    // 2. Read padding information
    unsigned char padding;
    in.read(reinterpret_cast<char *>(&padding), 1);

    // 3. Rebuild the exact Huffman Tree
    HuffmanTree tree;
    tree.buildTree(freq);

    ofstream out(outputFile, ios::binary);
    if (!out)
    {
        return false;
    }

    // 4. Calculate total bytes remaining to perfectly manage the final padding bits
    long currentPos = in.tellg();
    in.seekg(0, ios::end);
    long endPos = in.tellg();
    long compressedBytesCount = endPos - currentPos;
    in.seekg(currentPos, ios::beg);

    Node *current = tree.getRoot();
    unsigned char byte;

    // 5. Read compressed data byte-by-byte and process bits directly via fast bit shifting
    for (long i = 0; i < compressedBytesCount; ++i)
    {
        in.read(reinterpret_cast<char *>(&byte), 1);
        
        // If this is the absolute last byte, skip the padded junk bits
        int bitsToProcess = (i == compressedBytesCount - 1) ? (8 - padding) : 8;

        for (int b = 7; b >= (8 - bitsToProcess); --b)
        {
            // Extract the single bit at position 'b'
            int bit = (byte >> b) & 1;

            if (bit == 0)
            {
                current = current->left;
            }
            else
            {
                current = current->right;
            }

            // Once a leaf node is hit, write out the decoded character and reset to root
            if (current->left == nullptr && current->right == nullptr)
            {
                out.put(static_cast<char>(current->data));
                current = tree.getRoot();
            }
        }
    }

    out.close();
    in.close();
    return true;
}