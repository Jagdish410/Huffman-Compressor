#include "Compressor.h"
#include <fstream>

using std::string;
using std::ifstream;
using std::ofstream;
using std::ios;

string Compressor::compress(
    const string &filename,
    const std::unordered_map<unsigned char, string> &codes)
{
    ifstream file(filename, ios::binary);
    if (!file)
    {
        return "";
    }

    string encodedData;
    unsigned char byte;

    while (file.read(reinterpret_cast<char *>(&byte), 1))
    {
        encodedData += codes.at(byte);
    }

    file.close();
    return encodedData;
}

void Compressor::writeCompressedFile(
    const string &outputFile,
    const string &encodedData,
    const int freq[])
{
    ofstream out(outputFile, ios::binary);
    if (!out)
    {
        return;
    }

    // 1. Write metadata (Unique character count)
    int uniqueCount = 0;
    for (int i = 0; i < 256; i++)
    {
        if (freq[i] > 0)
        {
            uniqueCount++;
        }
    }

    out.write(reinterpret_cast<char *>(&uniqueCount), sizeof(int));

    // 2. Write character-frequency pairs
    for (int i = 0; i < 256; i++)
    {
        if (freq[i] > 0)
        {
            unsigned char ch = static_cast<unsigned char>(i);
            out.write(reinterpret_cast<char *>(&ch), sizeof(unsigned char));
            out.write(reinterpret_cast<const char *>(&freq[i]), sizeof(int));
        }
    }

    // 3. Calculate and write padding information
    int padding = (8 - (encodedData.size() % 8)) % 8;
    unsigned char paddingByte = static_cast<unsigned char>(padding);
    out.write(reinterpret_cast<char *>(&paddingByte), 1);

    // 4. Stream and pack bits using fast bitwise operations
    unsigned char bitBuffer = 0;
    int bitCount = 0;

    for (char bit : encodedData)
    {
        bitBuffer <<= 1;
        if (bit == '1')
        {
            bitBuffer |= 1;
        }
        bitCount++;

        if (bitCount == 8)
        {
            out.write(reinterpret_cast<char *>(&bitBuffer), 1);
            bitBuffer = 0;
            bitCount = 0;
        }
    }

    // Handle remaining bits at the end
    if (bitCount > 0)
    {
        bitBuffer <<= (8 - bitCount);
        out.write(reinterpret_cast<char *>(&bitBuffer), 1);
    }

    out.close();
}