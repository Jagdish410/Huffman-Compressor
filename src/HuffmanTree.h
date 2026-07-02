#ifndef HUFFMANTREE_H
#define HUFFMANTREE_H

#include <queue>
#include <vector>

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

    void print(Node* node, int depth) const;

public:
    HuffmanTree();

    void buildTree(const int freq[]);

    Node* getRoot() const;

    void printTree() const;
};

#endif