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
    for(size_t i=0;i<other.children.size();i++){
        addChild(*other.children[i]);
    }
    node = other.node;
    return *this;
}


const Tree& Tree::operator=(Tree &&other){
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

CycleTree& CycleTree::operator=(const CycleTree &other) {
    Tree::operator=(other);
    currCycle = other.currCycle;
    return *this;
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

MaxRankTree& MaxRankTree::operator=(const MaxRankTree &other) {
    Tree::operator=(other);
    return *this;
}

int MaxRankTree::findMaxRank() {
    if(Tree::getChildren().empty())  //base case
        return 0;
    size_t numChild = getChildren().size();
    int max = getChildren().size();
    for(size_t i = 0;i<numChild;i++){
        int c = ((MaxRankTree*)getChildren()[i])->findMaxRank();
        //find max number of kids in subtree recursively
        if(c>max)
            max =c;   //assign new max number
    }
    return max;
}

int MaxRankTree::findNode(int rank) {
    int numChild = getChildren().size();
    if(numChild==rank)
        //return first node with max children
        return Tree::getRoot();
   // size_t numChild = getChildren().size();
    int node=-1;
    for( int i = 0;i<numChild;i++){
        int child = ((MaxRankTree*)getChildren()[i])->findNode(rank);
        if(child!=-1)
            //found the node
            node=child;
    }
    return node;
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
    int rank = findMaxRank();  //find max children
    return findNode(rank);     //find node with max children
}

RootTree:: RootTree(int rootLabel1):Tree(rootLabel1){

}

RootTree& RootTree::operator=(const RootTree &other) {
    Tree::operator=(other);
    return *this;
}

Tree* RootTree::clone() const {
    return new RootTree(*this);
}

int RootTree::traceTree() {
    return getRoot();
}
