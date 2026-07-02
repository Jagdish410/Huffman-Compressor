#ifndef DECODER_H
#define DECODER_H

#include <string>

class Decoder
{
public:
    bool decompress(
        const std::string& inputFile,
        const std::string& outputFile);
};

#endif