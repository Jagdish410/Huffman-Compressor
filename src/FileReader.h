#ifndef FILEREADER_H
#define FILEREADER_H

#include <string>

class FileReader
{
private:
    int freq[256];

public:
    FileReader();

    bool readFile(const std::string &filename);

    const int *getFrequency() const;
};

#endif