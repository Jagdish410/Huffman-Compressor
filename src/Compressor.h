#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include <string>
#include <unordered_map>

class Compressor
{
public:
    std::string compress(
        const std::string& filename,
        const std::unordered_map<unsigned char, std::string>& codes);
};

#endif