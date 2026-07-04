#include <iostream>
#include <fstream>
#include <iomanip>

#include "FileReader.h"
#include "HuffmanTree.h"
#include "Compressor.h"
#include "Decoder.h"

using namespace std;

long long getFileSize(const string &filename)
{
    ifstream file(filename, ios::binary | ios::ate);

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
        cout << "Usage:\n";
        cout << "./huffman compress <input_file> <output_file>\n";
        cout << "./huffman decompress <input_file> <output_file>\n";
        return 1;
    }

    string mode = argv[1];
    string inputFile = argv[2];
    string outputFile = argv[3];

    if (mode == "compress")
    {
        FileReader reader;

        if (!reader.readFile(inputFile))
        {
            cout << "Unable to open input file.\n";
            return 1;
        }

        long long originalSize = getFileSize(inputFile);

        if (originalSize == 0)
        {
            cout << "Input file is empty.\n";
            return 1;
        }

        const int *freq = reader.getFrequency();

        HuffmanTree tree;
        tree.buildTree(freq);
        tree.generateCodes();

        Compressor compressor;

        string encodedData =
            compressor.compress(inputFile, tree.getCodes());

        compressor.writeCompressedFile(
            outputFile,
            encodedData,
            freq);

        long long compressedSize = getFileSize(outputFile);

        double ratio =
            (static_cast<double>(compressedSize) / originalSize) * 100.0;

        cout << fixed << setprecision(2);

        cout << "\nCompression Successful!\n\n";
        cout << "Input File       : " << inputFile << '\n';
        cout << "Output File      : " << outputFile << '\n';
        cout << "Original Size    : " << originalSize << " bytes\n";
        cout << "Compressed Size  : " << compressedSize << " bytes\n";
        cout << "Compression Ratio: " << ratio << "%\n";
    }
    else if (mode == "decompress")
    {
        Decoder decoder;

        if (decoder.decompress(inputFile, outputFile))
        {
            cout << "Decompression Successful!\n";
        }
        else
        {
            cout << "Decompression Failed!\n";
        }
    }
    else
    {
        cout << "Invalid mode.\n";
        return 1;
    }

    return 0;
}