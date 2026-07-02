#include <iostream>
#include "FileReader.h"
#include "HuffmanTree.h"
#include "Compressor.h"
#include "Decoder.h"

int main()
{
    FileReader reader;

    if (!reader.readFile("sample/sample.txt"))
    {
        std::cout << "Unable to open file.\n";
        return 1;
    }

    const int *freq = reader.getFrequency();

    HuffmanTree tree;

    tree.buildTree(freq);

    tree.generateCodes();

    Compressor compressor;

    std::string encodedData =
        compressor.compress("sample/sample.txt", tree.getCodes());

    compressor.writeCompressedFile(
        "output/sample.huff",
        encodedData,
        freq);

    std::cout << "Compression Successful!" << std::endl;

    Decoder decoder;

    if (decoder.decompress(
            "output/sample.huff",
            "output/sample_decoded.txt"))
    {
        std::cout << "Decoder successfully read compressed file." << std::endl;
    }
    else
    {
        std::cout << "Decoder failed." << std::endl;
    }

    return 0;
}