#include <iostream>
#include <fstream>
#include <iomanip>

#include "FileReader.h"
#include "HuffmanTree.h"
#include "Compressor.h"
#include "Decoder.h"

long long getFileSize(const std::string &filename)
{
    std::ifstream file(filename, std::ios::binary | std::ios::ate);

    if (!file)
    {
        return -1;
    }

    return file.tellg();
}

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        std::cout << "Usage:\n";
        std::cout << "./huffman compress <input_file> <output_file>\n";
        std::cout << "./huffman decompress <input_file> <output_file>\n";
        return 1;
    }

    std::string mode = argv[1];
    std::string inputFile = argv[2];
    std::string outputFile = argv[3];

    if (mode == "compress")
    {
        FileReader reader;

        if (!reader.readFile(inputFile))
        {
            std::cout << "Unable to open input file.\n";
            return 1;
        }

        const int *freq = reader.getFrequency();

        HuffmanTree tree;

        tree.buildTree(freq);

        tree.generateCodes();

        Compressor compressor;

        std::string encodedData =
            compressor.compress(inputFile, tree.getCodes());

        compressor.writeCompressedFile(
            outputFile,
            encodedData,
            freq);

        long long originalSize = getFileSize(inputFile);

        long long compressedSize = getFileSize(outputFile);

        double ratio =
            (static_cast<double>(compressedSize) / originalSize) * 100.0;

        std::cout << "\nCompression Successful!\n\n";

        std::cout << "Input File       : "
                  << inputFile
                  << '\n';

        std::cout << "Output File      : "
                  << outputFile
                  << '\n';

        std::cout << "Original Size    : "
                  << originalSize
                  << " bytes\n";

        std::cout << "Compressed Size  : "
                  << compressedSize
                  << " bytes\n";

        std::cout << std::fixed
                  << std::setprecision(2);

        std::cout << "Compression Ratio: "
                  << ratio
                  << "%\n";
    }
    else if (mode == "decompress")
    {
        Decoder decoder;

        if (decoder.decompress(inputFile, outputFile))
        {
            std::cout << "Decompression Successful!" << std::endl;
        }
        else
        {
            std::cout << "Decompression Failed!" << std::endl;
        }
    }
    else
    {
        std::cout << "Invalid mode.\n";
        return 1;
    }

    return 0;
}