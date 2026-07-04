#include "Decoder.h"
#include "HuffmanTree.h"

#include <fstream>

using std::ifstream;
using std::ios;
using std::ofstream;
using std::string;

bool Decoder::decompress(
    const string &inputFile,
    const string &outputFile)
{
    ifstream in(inputFile, ios::binary);

    if (!in)
    {
        return false;
    }

    int freq[256] = {0};

    int uniqueCount;

    if (!in.read(reinterpret_cast<char *>(&uniqueCount), sizeof(int)))
    {
        return false;
    }

    for (int i = 0; i < uniqueCount; i++)
    {
        unsigned char ch;
        int frequency;

        if (!in.read(reinterpret_cast<char *>(&ch), sizeof(unsigned char)))
        {
            return false;
        }

        if (!in.read(reinterpret_cast<char *>(&frequency), sizeof(int)))
        {
            return false;
        }

        freq[ch] = frequency;
    }

    unsigned char padding;

    if (!in.read(reinterpret_cast<char *>(&padding), 1))
    {
        return false;
    }

    HuffmanTree tree;
    tree.buildTree(freq);

    Node *root = tree.getRoot();

    if (root == nullptr)
    {
        return false;
    }

    ofstream out(outputFile, ios::binary);

    if (!out)
    {
        return false;
    }

    long currentPos = in.tellg();

    in.seekg(0, ios::end);
    long endPos = in.tellg();

    long compressedBytesCount = endPos - currentPos;

    in.seekg(currentPos, ios::beg);

    // Handle files containing only one unique character
    if (root->left == nullptr && root->right == nullptr)
    {
        for (int i = 0; i < 256; i++)
        {
            if (freq[i] > 0)
            {
                for (int j = 0; j < freq[i]; j++)
                {
                    out.put(static_cast<char>(i));
                }
                return true;
            }
        }
    }

    Node *current = root;
    unsigned char byte;

    for (long i = 0; i < compressedBytesCount; i++)
    {
        if (!in.read(reinterpret_cast<char *>(&byte), 1))
        {
            return false;
        }

        int bitsToProcess =
            (i == compressedBytesCount - 1) ? (8 - padding) : 8;

        for (int b = 7; b >= (8 - bitsToProcess); b--)
        {
            int bit = (byte >> b) & 1;

            if (bit == 0)
            {
                current = current->left;
            }
            else
            {
                current = current->right;
            }

            if (current == nullptr)
            {
                return false;
            }

            if (current->left == nullptr && current->right == nullptr)
            {
                out.put(static_cast<char>(current->data));
                current = root;
            }
        }
    }

    return true;
}