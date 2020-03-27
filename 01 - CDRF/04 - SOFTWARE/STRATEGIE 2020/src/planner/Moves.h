#ifndef MOVES_H
#define MOVES_H

#include "planner/Actions.h"

union Destination
{
    struct {
        float x, //x coordinate
              y, //y coordinate
              theta; //final absolute orientation
    }absolute;

    struct {
        float alpha, //turn
              dist,  //go
              theta; //final turn
    }relative;
};

class Move : public Action{
private:
    bool checkForOppenent;
    bool recalage;
    long speed;
    Destination dest;

public:
     Move ();
    ~Move ();

    virtual void cancel       () override; //function pointer for specific routine ?
    virtual void checkState   () override;
    virtual void exec         () override;
};

class Trajectory : Action{
private:

public:
     Trajectory();
    ~Trajectory();

    virtual void cancel       () override; //function pointer for specific routine ?
    virtual void checkState   () override;
    virtual void exec         () override;
};





#endif