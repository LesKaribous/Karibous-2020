#include "planner.h"

void initPlanner(){
    actionSize = 0;
    currentIndex - 1;
}

void planAction(action_t a){
    buffer[actionSize] = a;
    actionSize ++;
}

//A verifier
void cancelAction(int i){
    for(int n(0); n < actionSize; n++){
        if(n >= i){
            buffer[n] = buffer[n-1];
        }
    }
}

action_t getAction(int i){
    if(!(i >= 0  && i < actionSize)){
        action_t error;
        error.state = corrupted;
    }
    return buffer[i];
}

bool execBlock(action_t a){

    return true;
}

void checkState(action_t cb){

}

void handleFailure(action_t cb){

}

void handleSuccess(action_t cb){
    
}

void processBlocks(long timeLeft){
    //Process state
    action_t currentBlock = getAction(currentIndex);
    switch (currentBlock.state)
    {
        case blockState::scheduled :
            if(timeLeft > currentBlock.estDuration)
                execBlock(currentBlock);
            break;
        case blockState::started :
            checkState(currentBlock);
            break;
        case blockState::failed :
            handleFailure(currentBlock);
            break;
        case blockState::done :
            handleSuccess(currentBlock);
            currentIndex++;
            if(currentIndex == actionSize) finMatch();
            break;      
        default:
            break;
    }
}



void planAbsoluteMove(float X, float Y){

}

void planAbsoluteMove(float X, float Y, float alpha){

}

void planRelativeMove(float startAlpha, float distance){

}

void planRelativeMove(float startAlpha, float distance, float endAlpha){

}


