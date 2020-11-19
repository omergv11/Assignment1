#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>

class Graph{
public:
    Graph(std::vector<std::vector<int>> matrix);
    Graph(const Graph& other);
    Graph();    // empty constructor
    const Graph& operator=(const Graph &other);
    void infectNode(int nodeInd);
    bool isInfected(int nodeInd) const;
    std::vector<std::vector<int>> getMatrix() const;
    std::vector<int> getNeighbours(int node) const;
    std::vector<bool> getInfected() const;
    int getSize();
    void setEdges(const std::vector<std::vector<int>> newEdges);
    void spread(int node);
    bool hasSpread(int node) const;

private:
    std::vector<std::vector<int>> edges;
    int size;   // number of nodes
    std::vector<bool> infected;
    std::vector<bool> virus;
}
;
#endif
