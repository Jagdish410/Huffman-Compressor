#include "Compressor.h"

#include <fstream>

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