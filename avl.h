#ifndef AVL_H
#define AVL_H

#include <string>

struct AVLNode {
    int busId;
    double baseLoadP;
    double netDemand;
    int height;
    AVLNode* left;
    AVLNode* right;

    AVLNode(int id, double p, double demand) {
        busId = id;
        baseLoadP = p;
        netDemand = demand;
        height = 1;
        left = right = nullptr;
    }
};

class AVLTree {
private:
    AVLNode* root;
    int height(AVLNode* N);
    int getBalance(AVLNode* N);
    AVLNode* rightRotate(AVLNode* y);
    AVLNode* leftRotate(AVLNode* x);
    AVLNode* insert(AVLNode* node, int busId, double baseLoadP, double netDemand);
    AVLNode* searchNode(AVLNode* node, int busId);
    void destroyTree(AVLNode* node);

public:
    AVLTree();
    ~AVLTree();
    void loadFromCSV(const std::string& filepath);
    void search(int busId); // Public wrapper method
};

#endif