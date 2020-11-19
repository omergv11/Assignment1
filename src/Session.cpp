#include <iostream>
#include <fstream>
#include "../include/Session.h"
#include "../include/Agent.h"
#include "../include/json.hpp"
using json = nlohmann::json;
Session::Session(const std::string& path)
:g(),treeType(),agents(),infectedQ(),infectedList(),cycle(0) {
    std::ifstream temp (path);
    json input;
    temp >> input;
    Graph newG(input["graph"]);
    g=newG;
    //std::vector<std::vector<int>> matrix = input["Graph"];
    std::string tree = input["tree"];
    if(tree == "M"){
        treeType = MaxRank;
    }
    else if(tree=="C"){
        treeType = Cycle;
    }
    else if(tree=="R"){
        treeType = Root;
    }

    for(auto& elem:input["agents"]){
        if(elem[0]=="V"){
            Virus *v = new Virus(elem[1]);
            agents.push_back(v);
            //delete v;
        }
        else{
            ContactTracer* c = new ContactTracer();
            agents.push_back(c);
         //   delete c;
        }
    }

}

Session:: Session(const Session &other)
:g(),treeType(other.treeType),agents(),infectedQ(other.infectedQ),infectedList(),cycle(other.cycle){
    for(size_t i=0;i<other.agents.size();i++){
        Agent* newAgent = (*other.agents[i]).clone();
        agents.push_back(newAgent);
    }
    for(size_t i=0;i<other.infectedList.size();i++){
        infectedList.push_back(other.infectedList[i]);
    }
    g = other.g;
}

Session::Session(Session &&other)
:g(std::vector<std::vector<int>>()),treeType(other.treeType),agents(),infectedQ(other.infectedQ),infectedList(),cycle(other.cycle){
    g = std::move(other.g);
    treeType = other.treeType;
    agents = std::move(other.agents);
    infectedList = std::move(other.infectedList);
    infectedQ = std::move(other.infectedQ);
    other.cycle=0;
}
const Session& Session::operator=(Session &&other){
    if (this == &other)
        return *this;
    g = std::move(other.g);
    cycle = other.cycle;
    treeType = other.treeType;
    agents = std::move(other.agents);
    infectedList = std::move(other.infectedList);
    infectedQ = std::move(other.infectedQ);
    other.cycle=0;
    return *this;
}
Session& Session:: operator=(const Session& other) {
    if (this == &other) {
        return *this;
    }
    clear();
    g = other.g;
    for(size_t i=0;i<other.agents.size();i++){
        Agent* newAgent = (*other.agents[i]).clone();
        agents.push_back(newAgent);
    }
    cycle = other.cycle;
    infectedQ = other.infectedQ;
    treeType = other.treeType;
    return *this;

}
Session:: ~Session() {
    clear();
}
void Session::clear() {
    size_t numAgents = agents.size();
    for (size_t i=0; i<numAgents;i++){
        delete agents[i];
    }
    agents.clear();
}

void Session::simulate() {
    bool stop = false;
    while(!stop){
        stop = true;
        size_t size = agents.size();
        for(size_t i=0;i<size;i++){
            if(agents[i]->canAct(*this)){
                stop = false;
                agents[i]->act(*this);
            }
        }
        cycle++;
    }
    json output;
    output["graph"]=g.getMatrix();
    output["infected"]=infectedList;
    std::ofstream i("./output.json");
    i << output;
}

void Session::addAgent(const Agent& agent){
    Agent* newAgent = agent.clone();
    agents.push_back(newAgent);
}
void Session::setEdges(const std::vector<std::vector<int>> newEdges) {
    g.setEdges(newEdges);
}
void Session:: enqueueInfected(int node){

    infectedQ.push(node);
    infectedList.push_back(node);
}

void Session::spread(int node) {
    g.spread(node);
}

int Session:: dequeueInfected(){
    int pop = infectedQ.front();
    infectedQ.pop();
    return pop;
}

TreeType Session::getTreeType() const {
    return treeType;
}

Graph Session::getGraph() const{
    return g;
}

Tree* Session::BFS(int index) {
    Tree* bfsTree = Tree::createTree(*this,index);
     bool *visited = new bool[g.getSize()];
    for(int i=0;i<g.getSize();i++){
        visited[i] = false;
    }
    std::queue<Tree*> q;
    visited[index] = true;
    q.push(bfsTree);
    while(!q.empty()){
        Tree* curr = q.front();
        q.pop();
        std::vector<int> adj = g.getNeighbours(curr->getRoot());
        for(size_t i=0;i<adj.size();i++) {
            if (adj[i]==1 && !visited[i]) {
                Tree* child = Tree::createTree(*this,i);
                curr->addChild(*child);
                delete child;
                visited[i] = true;
                q.push(curr->getChildren().back());
            }
        }
    }
    delete[] (visited);
    visited=nullptr;
    return bfsTree;
}
int Session:: getCycle() const{
    return cycle;
}
void Session::infectNode(int nodeInd) {
    g.infectNode(nodeInd);
}

std::queue<int> Session::getInfectedQ() const {
    return infectedQ;
}

void Session::setGraph(const Graph &graph) {
    g = graph;
}
