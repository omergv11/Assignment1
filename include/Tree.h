#ifndef TREE_H_
#define TREE_H_

#include <vector>

class Session;

class Tree{
public:
    Tree(int rootLabel);                 //Constructor
    Tree(const Tree &tree);              //copy Constructor
    Tree (Tree &&other);                 //move Constructor
    Tree& operator=(const Tree &other);  //copy assignment operator
    const Tree& operator=(Tree &&other); //move assignment operator
    virtual ~Tree();                     //Destructor
    void clear();

    void addChild(const Tree& child);
    static Tree* createTree(const Session& session, int rootLabel);

    virtual std::vector<Tree*> getChildren() const;
    const int getRoot();

    virtual Tree* clone() const=0;
    virtual int traceTree()=0;

private:
    int node;
    std::vector<Tree*> children;
}
;
class CycleTree: public Tree{
public:
    CycleTree(int rootLabel, int currCycle);          //Constructor
    CycleTree& operator=(const CycleTree &other);     //copy assignment operator

    virtual Tree* clone() const;
    virtual int traceTree();

private:
    int currCycle;
}
;
class MaxRankTree: public Tree{
public:
    MaxRankTree(int rootLabel);                       //Constructor
    MaxRankTree& operator=(const MaxRankTree &other); //copy assignment operator

    int findMaxRank();
    int findNode(int rank);

    virtual std::vector<Tree*> getChildren() const;

    virtual Tree* clone() const;
    virtual int traceTree();
}
;
class RootTree: public Tree{
public:
    RootTree(int rootLabel1);                   //Constructor
    RootTree& operator=(const RootTree &other); //copy assignment operator

    virtual Tree* clone() const;
    virtual int traceTree();
}
;
#endif
