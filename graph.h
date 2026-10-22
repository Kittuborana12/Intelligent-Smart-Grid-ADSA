#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <vector>
#include <unordered_map>
#include <utility>

class SmartGridGraph {
private:
    std::unordered_map<int, std::vector<std::pair<int, double>>> adjList;

public:
    void addEdge(int u, int v, double weight);
    bool loadGraphFromCSV(const std::string& filepath);
    void printGraph();
    void BFS(int startBus);
    void DFS(int startBus);
    void shortestPathDijkstra(int startBus, int targetBus);
};

#endif