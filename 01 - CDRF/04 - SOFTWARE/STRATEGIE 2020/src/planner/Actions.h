#ifndef ACTIONS_H
#define ACTIONS_H

#include "planner/Moves.h"

#define MAX_ACTIONS_PER_BLOCK 50
#define INFINITE -1

enum State{ done,
            failed,
            started,
            scheduled,
            empty };


class Action{
//Innacessible from derived class
private:
    int id;
    Action *next;
//Accessible from derived class
protected:

    int scoreValue;
    long duration; //manual or auto ?
    bool cancelable;
    State state;

public:
     Action ();
    ~Action ();

    long  getDuration ();
    int   getId       ();
    int   getScore    ();
    State getState    ();
    
    bool  isCancellable ();
    bool  isStarted     ();
    bool  isEmpty       ();

    void setDuration  ();
    void setState     ();

    virtual void cancel       (); //function pointer for specific routine ?
    virtual void checkState   ();
    virtual void exec         ();
}; 


class Block : public Action{
    Action *first;
};




class Actuation : Action{
private:
    /* data */
    

public:
     Actuation ();
    ~Actuation ();

    virtual void cancel       () override; //function pointer for specific routine ?
    virtual void checkState   () override;
    virtual void exec         () override;
};


































#endif