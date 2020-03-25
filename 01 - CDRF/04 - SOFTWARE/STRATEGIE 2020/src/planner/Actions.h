#ifndef ACTIONS_H
#define ACTIONS_H

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

    bool  isCancellable();
    bool  isStarted();
    bool  isEmpty();

    void setDuration  ();
    void setState     ();

    void cancel       (); //function pointer for specific routine ?
    void checkState   ();
    void exec         ();
};

class Block : public Action{
    unsigned int index, size;
    Action actions[MAX_ACTIONS_PER_BLOCK];
};


class Move : public Action{
private:
    bool checkForOppenent;
    bool recalage;
    long speed;
public:
     Move ();
    ~Move ();
};

class Trajectory : Action{
private:

public:
     Trajectory();
    ~Trajectory();
};

class Actuation : Action{
private:
    /* data */
public:
     Actuation ();
    ~Actuation ();
};


































#endif