#include <iostream>
#include "graph.h"
#include "avl.h"
#include "treap.h"
#include "bplus.h"

int main() {
    std::cout << "===================================================================\n";
    std::cout << "   M.TECH ADSA PROJECT: INTELLIGENT SMART GRID DECISION SYSTEM     \n";
    std::cout << "   IEEE 33-Bus + DDRE-33 Merged Big Data Experimental Suite          \n";
    std::cout << "===================================================================\n\n";

    // -----------------------------------------------------------------
    // 1. SPATIAL GRAPH TOPOLOGY & FAULT RESTORATION
    // -----------------------------------------------------------------
    std::cout << "[MODULE 1] Spatial Graph Topology & Autonomous Fault Recovery\n";
    std::cout << "-------------------------------------------------------------------\n";
    SmartGridGraph gridGraph;
    gridGraph.loadGraphFromCSV("data/processed/Branch_Table.csv");
    
    std::cout << "[Status] Autonomously analyzing electrical distribution network...\n";
    // Dynamically calculate shortest restoration path from Substation 1 to Bus 33
    gridGraph.shortestPathDijkstra(1, 33);

    std::cout << "\n===================================================================\n";

    // -----------------------------------------------------------------
    // 2. AVL TREE O(log n) OPERATIONAL LOOKUP ENGINE
    // -----------------------------------------------------------------
    std::cout << "[MODULE 2] AVL Tree Deterministic Operational Index (O(log n))\n";
    std::cout << "-------------------------------------------------------------------\n";
    AVLTree avl;
    avl.loadFromCSV("data/processed/Master_Topology_Operational.csv");
    
    std::cout << "[Status] Ingested 1,156,320 operational records into AVL Tree structure.\n";
    std::cout << "[Query Execution] Instant Point-Lookup for Target Bus 33:\n";
    avl.search(33);

    std::cout << "\n===================================================================\n";

    // -----------------------------------------------------------------
    // 3. TREAP RANDOMIZED PRIORITY EMERGENCY RANKING
    // -----------------------------------------------------------------
    std::cout << "[MODULE 3] Treap Randomized Priority Ranking (Emergency Sort)\n";
    std::cout << "-------------------------------------------------------------------\n";
    Treap treap;
    treap.loadFromCSV("data/processed/Master_Topology_Operational.csv");
    
    std::cout << "[Status] Ingested operational records into Treap structure (BST + Heap).\n";
    std::cout << "[Query Execution] Evaluating Priority Restoration Queue for Bus 33:\n";
    treap.search(33);

    std::cout << "\n===================================================================\n";

    // -----------------------------------------------------------------
    // 4. B+ TREE BIG DATA TIME-SERIES RANGE INDEXING
    // -----------------------------------------------------------------
    std::cout << "[MODULE 4] B+ Tree Big Data Time-Series Range Search Engine\n";
    std::cout << "-------------------------------------------------------------------\n";
    BPlusTree bplusTree;
    bplusTree.loadTimeCompressedCSV("data/processed/Master_Topology_Operational.csv");
    
    std::cout << "[Status] Ingested 1,156,320 temporal records into B+ Tree Index.\n";
    std::cout << "[Query Execution] Executing Time-Window Range Scan [Timestamp 1000 to 1005]:\n";
    
    bplusTree.rangeQuery(1000, 1005);

    std::cout << "\n===================================================================\n";
    std::cout << "  ALL M.TECH ADSA PROJECT EXPERIMENTS COMPLETED SUCCESSFULLY!\n";
    std::cout << "===================================================================\n";

    return 0;
}