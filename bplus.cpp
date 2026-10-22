#include "bplus.h"

BPlusTree::BPlusTree() {
    root = new BPlusNode(true);
}

BPlusTree::~BPlusTree() {
    destroyTree(root);
}

void BPlusTree::destroyTree(BPlusNode* node) {
    if (!node) return;
    if (!node->isLeaf) {
        for (int i = 0; i <= node->numKeys; ++i) {
            destroyTree(node->children[i]);
        }
    }
    delete node;
}

void BPlusTree::insert(int key, double value) {
    BPlusNode* r = root;
    if (r->numKeys == MAX_CHILDREN - 1) {
        BPlusNode* s = new BPlusNode(false);
        root = s;
        s->children[0] = r;
        splitChild(0, s, r);
        insertNonFull(s, key, value);
    } else {
        insertNonFull(r, key, value);
    }
}

void BPlusTree::insertNonFull(BPlusNode* node, int key, double value) {
    int i = node->numKeys - 1;

    if (node->isLeaf) {
        while (i >= 0 && key < node->keys[i]) {
            node->keys[i + 1] = node->keys[i];
            node->values[i + 1] = node->values[i];
            i--;
        }
        node->keys[i + 1] = key;
        node->values[i + 1] = value;
        node->numKeys++;
    } else {
        while (i >= 0 && key < node->keys[i]) {
            i--;
        }
        i++;
        if (node->children[i]->numKeys == MAX_CHILDREN - 1) {
            splitChild(i, node, node->children[i]);
            if (key > node->keys[i]) {
                i++;
            }
        }
        insertNonFull(node->children[i], key, value);
    }
}

void BPlusTree::splitChild(int i, BPlusNode* parent, BPlusNode* child) {
    int t = MAX_CHILDREN / 2;
    BPlusNode* z = new BPlusNode(child->isLeaf);
    z->numKeys = MAX_CHILDREN - 1 - t;

    for (int j = 0; j < z->numKeys; j++) {
        z->keys[j] = child->keys[j + t];
        z->values[j] = child->values[j + t];
    }

    if (!child->isLeaf) {
        for (int j = 0; j <= z->numKeys; j++) {
            z->children[j] = child->children[j + t];
        }
    } else {
        z->next = child->next;
        child->next = z;
    }

    child->numKeys = t;

    for (int j = parent->numKeys; j >= i + 1; j--) {
        parent->children[j + 1] = parent->children[j];
    }
    parent->children[i + 1] = z;

    for (int j = parent->numKeys - 1; j >= i; j--) {
        parent->keys[j + 1] = parent->keys[j];
    }

    parent->keys[i] = child->keys[t - 1];
    parent->numKeys++;
}

bool BPlusTree::loadTimeCompressedCSV(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "[ERROR] Could not open Big Data CSV for B+ Tree: " << filepath << "\n";
        return false;
    }

    std::string line;
    std::getline(file, line); // Skip header: Timestamp_ID,Bus_ID,Base_Load_P,Base_Load_Q,Renewable_Generation,Net_Demand

    long long count = 0;
    while (std::getline(file, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string timeStr, busIdStr, loadPStr, loadQStr, genStr, demandStr;

        if (std::getline(ss, timeStr, ',') &&
            std::getline(ss, busIdStr, ',') &&
            std::getline(ss, loadPStr, ',') &&
            std::getline(ss, loadQStr, ',') &&
            std::getline(ss, genStr, ',') &&
            std::getline(ss, demandStr, ',')) {
            
            try {
                int timestampId = std::stoi(timeStr);
                double netDemand = std::stod(demandStr);

                // Index by Timestamp_ID for true Big Data temporal range indexing
                insert(timestampId, netDemand);
                count++;
            } catch (...) {}
        }
    }

    file.close();
    std::cout << "[SUCCESS] Ingested " << count << " temporal records into B+ Tree Big Data Index!\n";
    return true;
}

void BPlusTree::rangeQuery(int startKey, int endKey) {
    std::cout << "\n--- B+ Tree Big Data Time-Series Range Query [Timestamp " << startKey << " to " << endKey << "] ---\n";
    BPlusNode* curr = root;

    while (!curr->isLeaf) {
        curr = curr->children[0];
    }

    bool foundAny = false;
    long long matchedRecords = 0;
    while (curr != nullptr) {
        for (int i = 0; i < curr->numKeys; ++i) {
            int k = curr->keys[i];
            if (k >= startKey && k <= endKey) {
                if (matchedRecords < 10) { // Print first 10 matches for preview
                    std::cout << " -> Timestamp ID: " << k << " | Net Demand Payload: " << curr->values[i] << " kW\n";
                }
                foundAny = true;
                matchedRecords++;
            }
        }
        curr = curr->next;
    }

    if (foundAny) {
        std::cout << "[INFO] Total matching time-series records found in range: " << matchedRecords << "\n";
    } else {
        std::cout << "[INFO] No records found in the specified timestamp range.\n";
    }
}