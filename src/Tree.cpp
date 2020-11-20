#include <iostream>
#include "../include/Tree.h"
#include "../include/Session.h"

Tree::Tree(int rootLabel)
        :node(rootLabel),children(){
}

Tree::Tree(const Tree &tree)
        :node(tree.node),children() {
    for(size_t i=0;i<tree.children.size();i++){
        addChild(*tree.children[i]);
    }
}

Tree:: Tree(Tree &&other):node(other.node),children(){
    children = std::move(other.children);
    other.node=0;
}

Tree& Tree:: operator=(const Tree &other) {
    clear();
    for(size_t i=0;i<other.children.size();i++){
        addChild(*other.children[i]);
    }
    node = other.node;
    return *this;
}


Tree& Tree::operator=(Tree&& other) {
    if(this==&other)
        return *this;
    clear();
    children = std::move(other.children);
    other.children.clear();
    node = other.node;
    other.node=0;
    return *this;
}

Tree::~Tree() {
    clear();
}

void Tree::clear() {
    size_t numChild = children.size();
    for (size_t i=0; i<numChild;i++){
        delete children[i]; //delete every child
    }
    children.clear();
}

void Tree::addChild(const Tree &child) {
    Tree* tr = child.clone();
    children.push_back(tr);
}

Tree * Tree::createTree(const Session &session, int rootLabel) {
    Tree* newTree;
    //decide according to tree type
    if(session.getTreeType()==MaxRank){
        newTree = new MaxRankTree(rootLabel);
    }
    else if(session.getTreeType()==Cycle){
        newTree = new CycleTree(rootLabel,session.getCycle());
    }
    else if (session.getTreeType()==Root){
        newTree = new RootTree(rootLabel);
    }
    return newTree;
}

std::vector<Tree*> Tree::getChildren() const{
    return children;
}

const int Tree::getRoot() {
    return node;
}

CycleTree:: CycleTree(int rootLabel1,int currCycle)
        :Tree(rootLabel1),currCycle(currCycle){

}

Tree* CycleTree::clone() const {
    return new CycleTree(*this);
}

int CycleTree::traceTree() {
    Tree* pt = this;
    for(int i=0;i<currCycle ;i++){
        //go over tree cycle times or return the last
        if(pt->getChildren().empty()) {
            break;
        }
        pt = pt->getChildren()[0]; //go to most left child

    }
    return pt->getRoot();
}

MaxRankTree:: MaxRankTree(int rootLabel1):Tree(rootLabel1){

}

std::pair<int,int> MaxRankTree::findMaxRank() {
    if(Tree::getChildren().empty())  //base case
        return std::pair<int,int>(0,getRoot());
    size_t numChild = getChildren().size();
    std::pair<int,int> max(getChildren().size(),getRoot());
    for(size_t i = 0;i<numChild;i++){
        std::pair<int,int> c = ((MaxRankTree*)getChildren()[i])->findMaxRank();
        //find max number of kids and their index in subtree recursively
        if(c.first>max.first)
            max =c;   //assign new node with max children
    }
    return max;
}

std::vector<Tree*> MaxRankTree:: getChildren() const{
    return Tree::getChildren();
}

Tree* MaxRankTree::clone() const {
    return new MaxRankTree(*this);
}

int MaxRankTree::traceTree() {
    if(getChildren().empty())  //base case
        return getRoot();
    std::pair<int,int> rank = findMaxRank();  //find node to disconnect
    return rank.second;     //return nodes index
}

RootTree:: RootTree(int rootLabel1):Tree(rootLabel1){

}

Tree* RootTree::clone() const {
    return new RootTree(*this);
}

int RootTree::traceTree() {
    return getRoot();
}
