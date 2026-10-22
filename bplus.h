#ifndef BPLUS_H
#define BPLUS_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

const int MAX_CHILDREN = 4;

struct BPlusNode {
    bool isLeaf;
    int numKeys;
    int keys[MAX_CHILDREN];
    double values[MAX_CHILDREN]; // Stores Net Demand or operational values
    BPlusNode* children[MAX_CHILDREN + 1];
    BPlusNode* next; // Pointer to next leaf for fast range scans

    BPlusNode(bool leaf) {
        isLeaf = leaf;
        numKeys = 0;
        next = nullptr;
        for (int i = 0; i < MAX_CHILDREN + 1; ++i) {
            children[i] = nullptr;
        }
    }
};

class BPlusTree {
private:
    BPlusNode* root;
    void splitChild(int i, BPlusNode* parent, BPlusNode* child);
    void insertNonFull(BPlusNode* node, int key, double value);
    void destroyTree(BPlusNode* node);

public:
    BPlusTree();
    ~BPlusTree();
    void insert(int key, double value);
    bool loadTimeCompressedCSV(const std::string& filepath);
    void rangeQuery(int startKey, int endKey);
};

#endif // BPLUS_H
