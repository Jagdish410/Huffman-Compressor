#include <iostream>
#include "FileReader.h"
#include "HuffmanTree.h"
#include "Compressor.h"

int main()
{
    FileReader reader;

    if (!reader.readFile("sample/sample.txt"))
    {
        std::cout << "Unable to open file.\n";
        return 1;
    }

    const int* freq = reader.getFrequency();

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

    return 0;
}