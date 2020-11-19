//
// Created by spl211 on 02/11/2020.
//
#include <iostream>
#include "../include/Graph.h"
Graph::Graph() : edges(),size(),infected(),virus() {}
Graph::Graph(std::vector<std::vector<int>> matrix): edges(),size(),infected(),virus() {
    size = matrix.size();
    edges = matrix;
    for (int i=0;i<size;i++){
        infected.push_back(false);
        virus.push_back((false));
    }
}
Graph:: Graph(const Graph& other)
        :edges(other.edges), size(other.size),infected(other.infected),virus(other.virus) {

}
const Graph& Graph:: operator=(const Graph &other) {
    if (this == &other) {
        return *this;
    }

    infected.clear();
    virus.clear();
    for (size_t i=0;i<other.infected.size();i++){
        infected.push_back(other.infected[i]);
        virus.push_back(other.virus[i]);
    }
    edges = other.edges;
    size = other.size;
    return *this;

}
void Graph:: infectNode(int nodeInd) {
    infected[nodeInd] = true;
}

void Graph::setEdges(const std::vector<std::vector<int>> newEdges){
    for(size_t i=0;i<edges.size();i++){
        for(size_t j=0;j<edges[i].size();j++){
            edges[i][j] = newEdges[i][j];
        }
    }
}

bool Graph:: isInfected(int nodeInd) const {
    return infected[nodeInd];
}
std::vector<std::vector<int>> Graph:: getMatrix() const {
    return edges;
}
std::vector<int> Graph:: getNeighbours(int node) const{
    return edges[node];
}
std::vector<bool> Graph:: getInfected() const{
    return infected;
}
int Graph::getSize() {
    return size;
}

void Graph::spread(int node) {
    virus[node] = true;
}

bool Graph::hasSpread(int node) const{
    return virus[node];
}
