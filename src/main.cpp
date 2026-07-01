#include <iostream>
#include "FileReader.h"

int main()
{
    FileReader reader;

    if (!reader.readFile("sample/sample.txt"))
    {
        std::cout << "Unable to open file.\n";
        return 1;
    }

    const int* freq = reader.getFrequency();

    for (int i = 0; i < 256; i++)
    {
        if (freq[i] > 0)
        {
            std::cout << (char)i << " : " << freq[i] << '\n';
        }
    }

    return 0;
}