#ifndef PLANNER_H
#define PLANNER_H

#define MAX_BUFFER_SIZE 100
#define MAX_PARAMS 5

/****************************
 *         Typedef          *
 ****************************/

enum blockType {  actuator,
                  absMove,
                  relMove };

enum blockState{  done,
                  failed,
                  started,
                  scheduled,
                  corrupted };

//template <class T>
struct action_t {
    //T parameters[MAX_PARAM];
    float parameters[MAX_PARAMS];

    blockType type;
    blockState state;

    long estDuration;
    bool checkForOppenent;
    bool cancelable;
    
    int id;
    int scoreValue;
};

/****************************
 *        Variables         *
 ****************************/

action_t buffer[MAX_BUFFER_SIZE];
int currentIndex,
    actionSize;

/****************************
 *        Methods           *
 ****************************/

void initPlanner        ();
bool execBlock          (action_t);
void processBlocks      (long);

void handleSuccess      (action_t cb);
void handleFailure      (action_t cb);



void planAction         (action_t);
void cancelAction       (int);
action_t getAction      (int);

void planAbsoluteMove   (float X, float Y);
void planAbsoluteMove   (float X, float Y, float alpha);

void planRelativeMove   (float startAlpha, float distance);
void planRelativeMove   (float startAlpha, float distance, float endAlpha);

void planServoMove      (float angle);

void finMatch(){};



#endif