#ifndef HUFFMANTREE_H
#define HUFFMANTREE_H

#include <queue>
#include <vector>
#include <unordered_map>
#include <string>

class Node
{
public:
    unsigned char data;
    int frequency;

    Node* left;
    Node* right;

    Node(unsigned char d, int f);
};

class Compare
{
public:
    bool operator()(Node* a, Node* b)
    {
        return a->frequency > b->frequency;
    }
};

class HuffmanTree
{
private:
    Node* root;

    std::unordered_map<unsigned char, std::string> codes;

    void print(Node* node, int depth) const;

    void generateCodes(Node* node, std::string code);

public:
    HuffmanTree();

    void buildTree(const int freq[]);

    Node* getRoot() const;

    void printTree() const;

    void generateCodes();

    const std::unordered_map<unsigned char, std::string>& getCodes() const;
};

#endif