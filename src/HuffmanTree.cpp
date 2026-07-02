#include <iostream>
#include "HuffmanTree.h"

Node::Node(unsigned char d, int f)
{
    data = d;
    frequency = f;

    left = nullptr;
    right = nullptr;
}

HuffmanTree::HuffmanTree()
{
    root = nullptr;
}

Node *HuffmanTree::getRoot() const
{
    return root;
}


void HuffmanTree::buildTree(const int freq[])
{
    std::priority_queue<Node *, std::vector<Node *>, Compare> pq;

    for (int i = 0; i < 256; i++)
    {
        if (freq[i] > 0)
        {
            pq.push(new Node((unsigned char)i, freq[i]));
        }
    }
    while (pq.size() > 1)
    {
        Node *left = pq.top();
        pq.pop();

        Node *right = pq.top();
        pq.pop();

        Node *parent = new Node('\0', left->frequency + right->frequency);

        parent->left = left;
        parent->right = right;

        pq.push(parent);
    }

    if (!pq.empty())
    {
        root = pq.top();
    }
}

void HuffmanTree::printTree() const
{
    print(root, 0);
}

void HuffmanTree::print(Node* node, int depth) const
{
    if (node == nullptr)
    {
        return;
    }

    for (int i = 0; i < depth; i++)
    {
        std::cout << "    ";
    }

    if (node->left == nullptr && node->right == nullptr)
    {
        std::cout << node->data << " (" << node->frequency << ")" << std::endl;
    }
    else
    {
        std::cout << "* (" << node->frequency << ")" << std::endl;
    }

    print(node->left, depth + 1);
    print(node->right, depth + 1);
}

void HuffmanTree::generateCodes()
{
    codes.clear();

    generateCodes(root, "");
}

void HuffmanTree::generateCodes(Node* node, std::string code)
{
    if (node == nullptr)
    {
        return;
    }

    // Leaf node
    if (node->left == nullptr && node->right == nullptr)
    {
        codes[node->data] = code;
        return;
    }

    generateCodes(node->left, code + "0");

    generateCodes(node->right, code + "1");
}

const std::unordered_map<unsigned char, std::string>& HuffmanTree::getCodes() const
{
    return codes;
}

HuffmanTree::~HuffmanTree()
{
    deleteTree(root);
}

void HuffmanTree::deleteTree(Node* node)
{
    if (node == nullptr)
    {
        return;
    }

    deleteTree(node->left);

    deleteTree(node->right);

    delete node;
}