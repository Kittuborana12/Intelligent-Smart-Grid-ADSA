#ifndef TREAP_H
#define TREAP_H

#include <string>

struct TreapNode {
    int busId;
    double baseLoadP;
    double netDemand;
    int priority;
    TreapNode* left;
    TreapNode* right;

    TreapNode(int id, double p, double demand) {
        busId = id;
        baseLoadP = p;
        netDemand = demand;
        priority = rand() % 10000; // Randomized heap priority
        left = right = nullptr;
    }
};

class Treap {
private:
    TreapNode* root;
    TreapNode* rightRotate(TreapNode* y);
    TreapNode* leftRotate(TreapNode* x);
    TreapNode* insert(TreapNode* root, int busId, double baseLoadP, double netDemand);
    TreapNode* searchNode(TreapNode* root, int busId);
    void destroyTree(TreapNode* node);

public:
    Treap();
    ~Treap();
    void loadFromCSV(const std::string& filepath);
    void search(int busId);
};

#endif