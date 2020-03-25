/*    ______ __                               
*    |   __ \  |.---.-.-----.-----.-----.----.
*    |    __/  ||  _  |     |     |  -__|   _|
*    |___|  |__||___._|__|__|__|__|_____|__|  
*/                                         

#ifndef PLANNER_H
#define PLANNER_H

#define MAX_BUFFER_SIZE 100

/****************************
 *         Typedef          *
 ****************************/


/****************************
 *      Planner class       *
 ****************************/

static class Planner{
public :
//  ___Constructor___

    Planner();

//  _____Methods_____

    void initPlanner        ();

//Action methods

    void planAction         (Action);
    void checkAction        (Action);

    void handleSuccess      (Action);
    void handleFailure      (Action); //Useful for relative mode

    //Get
    Action getAction      (int);

//Block Methods
    void planBlock          (block_t);
    void processBlocks      (long);
    bool execBlock          (Action);
    void checkBlock         (block_t cb);
    
    void handleBlockSuccess (block_t cb);
    void handleBlockFailure (block_t cb); //Useful for relative mode

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
    block_t buffer[MAX_BUFFER_SIZE];

    int blockIndex,
        blockCount;

} planner;


/****************************
 *        Variables         *
 ****************************/

//TODO remove that
void finMatch(){};



#endif