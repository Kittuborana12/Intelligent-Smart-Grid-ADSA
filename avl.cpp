#include "avl.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>

AVLTree::AVLTree() : root(nullptr) {}

AVLTree::~AVLTree() {
    destroyTree(root);
}

void AVLTree::destroyTree(AVLNode* node) {
    if (node) {
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }
}

int AVLTree::height(AVLNode* N) {
    return N ? N->height : 0;
}

int AVLTree::getBalance(AVLNode* N) {
    return N ? height(N->left) - height(N->right) : 0;
}

AVLNode* AVLTree::rightRotate(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;
    x->right = y;
    y->left = T2;
    y->height = std::max(height(y->left), height(y->right)) + 1;
    x->height = std::max(height(x->left), height(x->right)) + 1;
    return x;
}

AVLNode* AVLTree::leftRotate(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;
    y->left = x;
    x->right = T2;
    x->height = std::max(height(x->left), height(x->right)) + 1;
    y->height = std::max(height(y->left), height(y->right)) + 1;
    return y;
}

AVLNode* AVLTree::insert(AVLNode* node, int busId, double baseLoadP, double netDemand) {
    if (!node) return new AVLNode(busId, baseLoadP, netDemand);

    if (busId < node->busId)
        node->left = insert(node->left, busId, baseLoadP, netDemand);
    else if (busId > node->busId)
        node->right = insert(node->right, busId, baseLoadP, netDemand);
    else {
        node->netDemand = netDemand;
        return node;
    }

    node->height = 1 + std::max(height(node->left), height(node->right));
    int balance = getBalance(node);

    // Left Left
    if (balance > 1 && busId < node->left->busId)
        return rightRotate(node);
    // Right Right
    if (balance < -1 && busId > node->right->busId)
        return leftRotate(node);
    // Left Right
    if (balance > 1 && busId > node->left->busId) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    // Right Left
    if (balance < -1 && busId < node->right->busId) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

void AVLTree::loadFromCSV(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) return;

    std::string line;
    std::getline(file, line); // Skip header

    int count = 0;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string segment;
        std::vector<std::string> row;

        while (std::getline(ss, segment, ',')) {
            segment.erase(remove(segment.begin(), segment.end(), '\r'), segment.end());
            row.push_back(segment);
        }

        if (row.size() >= 6) {
            try {
                int busId = std::stoi(row[1]);
                double baseLoadP = std::stod(row[2]);
                double netDemand = std::stod(row[5]);
                root = insert(root, busId, baseLoadP, netDemand);
                count++;
            } catch (...) {
                continue;
            }
        }
    }
    file.close();
    std::cout << "[SUCCESS] Loaded operational records into AVL Tree (O(log n) indexed)!\n";
}

AVLNode* AVLTree::searchNode(AVLNode* node, int busId) {
    if (!node || node->busId == busId) return node;
    if (node->busId > busId) return searchNode(node->left, busId);
    return searchNode(node->right, busId);
}

void AVLTree::search(int busId) {
    AVLNode* result = searchNode(root, busId);
    if (result) {
        std::cout << "  -> Status          : Found in AVL Index (O(log n) Balanced)\n";
        std::cout << "  -> Target Bus ID   : " << result->busId << "\n";
        std::cout << "  -> Base Load P     : " << result->baseLoadP << " kW\n";
        std::cout << "  -> Net Demand      : " << result->netDemand << " kW\n";
    } else {
        std::cout << "  -> Status          : Bus ID " << busId << " not found in AVL Tree.\n";
    }
}