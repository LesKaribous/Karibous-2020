#ifndef PLANNER_H
#define PLANNER_H

#define MAX_BUFFER_SIZE 100
#define MAX_PARAMS 5

/****************************
 *         Typedef          *
 ****************************/

enum ActionType { servo,
                  absMove,
                  relMove };

enum State      { done,
                  failed,
                  started,
                  scheduled };

//template <class T>
struct action_t {
    //T parameters[MAX_PARAM];
    float parameters[MAX_PARAMS];

    ActionType blockType;
    State blockState;

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
int currentAction,
    actionSize;



/****************************
 *        Methods           *
 ****************************/

void initPlanner();
void processActions(long timeLeft);

void planAction(action_t a);
void cancelAction(int i);
action_t getAction(int i);

void planAbsoluteMove(float X, float Y);
void planAbsoluteMove(float X, float Y, float alpha);

void planRelativeMove(float startAlpha, float distance);
void planRelativeMove(float startAlpha, float distance, float endAlpha);

void planServoMove(float angle);




#endif