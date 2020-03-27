//     ______ __                               
//    |   __ \  |.---.-.-----.-----.-----.----.
//    |    __/  ||  _  |     |     |  -__|   _|
//    |___|  |__||___._|__|__|__|__|_____|__|  

#ifndef PLANNER_H
#define PLANNER_H
                                         
#include "planner/Actions.h"

#define MAX_BUFFER_SIZE 100


/****************************
 *      Planner class       *
 ****************************/

static class Planner{
public :
//  ___Constructor___

     Planner();
    ~Planner();

//  _____Methods_____

    void initPlanner        (); // TODO remove this

//Action methods

    void planAction         (Action);
    void checkAction        (Action);

    void handleSuccess      (Action);
    void handleFailure      (Action); //Useful for relative mode

    //Get
    Action getAction      (int);

//Block Methods
    void planBlock          (Block);
    void processBlocks      (long);
    bool execBlock          (Block);
    void checkBlock         (Block cb);
    
    void handleBlockSuccess (Block cb);
    void handleBlockFailure (Block cb); //Useful for relative mode

    void cancelBlockAction  (int);
    void cancelAction       (int blockIndex, int actionIndex);

    Action getBlockAction (int);

//Plan function
    void planAbsoluteMove   (float X, float Y);
    void planAbsoluteMove   (float X, float Y, float alpha);

    void planRelativeMove   (float startAlpha, float distance);
    void planRelativeMove   (float startAlpha, float distance, float endAlpha);

    void planServoMove      (float angle);

//  _____Attributes_____
 
protected :
    Block buffer[MAX_BUFFER_SIZE];
    int blockIndex,
        blockCount;

} planner;


/****************************
 *        Variables         *
 ****************************/

//TODO remove that
void finMatch(){};



#endif