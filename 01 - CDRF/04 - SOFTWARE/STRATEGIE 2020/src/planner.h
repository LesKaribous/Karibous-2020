#ifndef PLANNER_H
#define PLANNER_H

#define MAX_BUFFER_SIZE 100
#define MAX_ACTIONS_PER_BLOCK 50
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

    long duration; //manual & auto
    bool checkForOppenent;
    bool cancelable;
    
    int id;
    int scoreValue;
};

struct block_t{
    action_t actions[MAX_ACTIONS_PER_BLOCK];
    long duration; //manual & auto
    bool cancelable;
    int id;
    int scoreValue;

    blockState state;
    int index,
        actionsCount;
};



/****************************
 *        Variables         *
 ****************************/

block_t buffer[MAX_BUFFER_SIZE];

int blockIndex,
    blockCount;

/****************************
 *        Methods           *
 ****************************/

void initPlanner        ();
bool execBlock          (action_t);
void processBlocks      (long);

void handleSuccess      (action_t cb);
void handleFailure      (action_t cb); //Useful for relative mode

void planAction         (action_t);

void cancelBlockAction  (int);
void cancelAction       (int blockIndex, int actionIndex);

action_t getBlockAction (int);
action_t getAction      (int);

void planAbsoluteMove   (float X, float Y);
void planAbsoluteMove   (float X, float Y, float alpha);

void planRelativeMove   (float startAlpha, float distance);
void planRelativeMove   (float startAlpha, float distance, float endAlpha);

void planServoMove      (float angle);

void finMatch(){};



#endif