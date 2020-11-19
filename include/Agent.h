#ifndef AGENT_H_
#define AGENT_H_

#include <vector>
#include "../include/Session.h"

class Agent{
public:
    Agent();          //default
    virtual ~Agent(); //default destructor
    virtual bool canAct(Session& session) =0;
    virtual void act(Session& session)=0;
    virtual Agent* clone() const=0;
    virtual int getNode() const=0;
};
class ContactTracer: public Agent{
public:
    ContactTracer();                          //default constructor
    ContactTracer(const ContactTracer &other);//copy constructor

    virtual bool canAct(Session& session) ;
    virtual void act(Session& session);
    virtual Agent* clone() const;
    virtual int getNode() const;
};

class Virus: public Agent{
public:
Virus(int nodeInd);              //constructor
Virus(const Virus &other);       //copy constructor

virtual bool canAct(Session& session);
virtual void act(Session& session);
virtual Agent* clone() const;
virtual int getNode() const;

bool check(const Graph &g);
int next(const Graph &g);

private:
const int nodeInd;
}
;
#endif
