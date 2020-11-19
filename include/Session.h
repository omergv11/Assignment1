#ifndef SESSION_H_
#define SESSION_H_

#include <vector>
#include <string>
#include "Graph.h"
#include "Tree.h"
#include <queue>

class Agent;

enum TreeType{
  Cycle,
  MaxRank,
  Root
};

class Session{
public:
    Session(const std::string& path);  //constructor
    Session(const Session &other); //copy constructor
    Session& operator=(const Session& other); // assignment operator
    virtual ~Session(); // destructor
    Session (Session &&other); // move constructor
    const Session& operator=(Session &&other); // move assignment operator
    void clear(); // deletes the resources

    void simulate();
    void addAgent(const Agent& agent);
    void infectNode(int nodeInd); // change the value of the 'nodeInd'th element of the 'infected' vector in Graph to true
    void enqueueInfected(int);
    int dequeueInfected();
    Tree* BFS(int index); // creates the BFS tree, starting form the node 'index', that is needed for the contact tracer

    TreeType getTreeType() const;
    Graph getGraph() const;
    int getCycle() const;
    std::queue<int> getInfectedQ() const;

    void setEdges(const std::vector<std::vector<int>> newEdges); // set a new edges matrix in 'g'
    void spread(int node); // change the value of the 'node'th element of the 'virus' vector in Graph
    void setGraph (const Graph& graph);

private:
    Graph g;
    TreeType treeType;
    std::vector<Agent*> agents;
    std::queue<int> infectedQ; // the 'infected queue'
    std::vector<int> infectedList; // all the nodes that have been infected during the running of the program
    int cycle; // indicates which cycle of the program we're in (used to create CycleTree)
}
;
#endif

