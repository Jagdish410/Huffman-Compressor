#include "FileReader.h"

#include <fstream>

FileReader::FileReader()
{
    for (int i = 0; i < 256; i++)
    {
        freq[i] = 0;
    }
}

bool FileReader::readFile(const std::string &filename)
{
    for (int i = 0; i < 256; i++)
    {
        freq[i] = 0;
    }

    std::ifstream file(filename, std::ios::binary);

    if (!file)
    {
        return false;
    }

    unsigned char byte;

    while (file.read(reinterpret_cast<char *>(&byte), 1))
    {
        freq[byte]++;
    }

    return true;
}

const int *FileReader::getFrequency() const
{
    return freq;
}