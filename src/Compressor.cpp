#include "Compressor.h"

#include <fstream>
#include <bitset>

std::string Compressor::compress(
    const std::string& filename,
    const std::unordered_map<unsigned char, std::string>& codes)
{
    std::ifstream file(filename, std::ios::binary);

    if (!file)
    {
        return "";
    }

    std::string encodedData;

    unsigned char byte;

    while (file.read(reinterpret_cast<char*>(&byte), 1))
    {
        encodedData += codes.at(byte);
    }

    file.close();

    return encodedData;
}

void Compressor::writeCompressedFile(
    const std::string& outputFile,
    const std::string& encodedData,
    const int freq[])
{
    std::ofstream out(outputFile, std::ios::binary);

    if (!out)
    {
        return;
    }

    // Write frequency table (256 integers)
    out.write(reinterpret_cast<const char*>(freq), 256 * sizeof(int));

    std::string bits = encodedData;

    // Calculate padding
    int padding = (8 - (bits.size() % 8)) % 8;

    // Write padding information
    unsigned char paddingByte = static_cast<unsigned char>(padding);
    out.write(reinterpret_cast<char*>(&paddingByte), 1);

    // Add padding bits
    while (bits.size() % 8 != 0)
    {
        bits += '0';
    }

    // Write compressed data
    for (size_t i = 0; i < bits.size(); i += 8)
    {
        std::bitset<8> byte(bits.substr(i, 8));

        unsigned char value =
            static_cast<unsigned char>(byte.to_ulong());

        out.write(reinterpret_cast<char*>(&value), 1);
    }

    out.close();
}