//
// Created by spl211 on 02/11/2020.
//
#include <iostream>
#include "../include/Agent.h"

Agent::Agent() {}
Agent::~Agent() noexcept {}

ContactTracer::ContactTracer(){};
ContactTracer::ContactTracer(const ContactTracer &other) {
}

bool ContactTracer::canAct(Session &session) {
    if (session.getInfectedQ().empty())
        //base case
        return false;
    bool canAct = false;
    int node = session.getInfectedQ().front();
    for (unsigned int i = 0; i < session.getGraph().getNeighbours(node).size(); i++) {
        if (session.getGraph().getNeighbours(node)[i] == 1) {
            if (!session.getGraph().getInfected()[i])    //if there's a neighbour which is not infected
            {
                canAct = true;
                break;
            }
        }
    }
    if (!canAct) {
        session.dequeueInfected();
    }
    return canAct;
}
void ContactTracer::act(Session &session) {
    int infected = session.dequeueInfected();
    Tree* tree = session.BFS(infected); //make BFS tree from infected node
    int disc = tree->traceTree();       //and get the node to disconnect
    std::vector<std::vector<int>> newG = session.getGraph().getMatrix();
    for(unsigned int i =0; i<newG.size();i++){
        //remove relevant edges
        newG[disc][i]=0;
        newG[i][disc]=0;
    }
    session.setEdges(newG); //update graph
    delete tree;
}

Agent* ContactTracer:: clone() const{
    return new ContactTracer(*this);
}

int ContactTracer::getNode() const{ return -1;}

Virus::Virus(int nodeInd): nodeInd(nodeInd){
}

Virus:: Virus(const Virus &other):nodeInd(other.nodeInd){}


bool Virus::canAct(Session &session) {
    if(!session.getGraph().isInfected(nodeInd))
        //if node is not infected, then he can be
        return true;
    return check(session.getGraph());
}

void Virus::act(Session &session) {
    if(!session.getGraph().isInfected(nodeInd)){
        //infect the node and add him to list
        session.infectNode(nodeInd);
        session.enqueueInfected(nodeInd);
    }
    int nextNode = next(session.getGraph());
    if(nextNode!=-1){
        //if we can continue, make a new agent and spread the virus
        Virus *v = (new Virus(nextNode));
        session.addAgent(*v);
        delete v;
        session.spread(nextNode);

    }

}

Agent* Virus:: clone() const{
    return new Virus(*this);
}

int Virus::getNode() const {
    return nodeInd;
}

bool Virus::check(const Graph &g) {
    bool check = false;
    //checking if virus can act
        for(unsigned int i=0;i<g.getMatrix()[nodeInd].size();i++){
            if(g.getMatrix()[nodeInd][i]==1){
                if(!g.getInfected()[i] && !g.hasSpread(i))
                    //only if node is not infected or has the virus
                {
                    check=true;
                    break;
                }
            }
        }

    return check;
}

int Virus::next(const Graph &g) {
    int next=-1;
    for(unsigned int i=0;i<g.getMatrix()[nodeInd].size();i++){
        if(g.getMatrix()[nodeInd][i]==1){
            if(!g.isInfected(i)&&!g.hasSpread(i)) {
                //go over node's neighbours and check if can infect them
                next = i;
                break;
            }
        }
    }

    return next;
}


