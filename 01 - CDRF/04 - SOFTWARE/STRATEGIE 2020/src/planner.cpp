#include "planner.h"

/**
 * @brief inititialize planner
 */
void initPlanner(){
    blockCount = 0;
    blockIndex = 0;
}

/**
 * @brief add a new block to the queue
 */
void planBlock(block_t a){
    buffer[blockCount] = a;
    blockCount ++;
}

/**
 * @brief Add a new actions to the last block
 */
void planAction(action_t a){
    block_t currentBlock = buffer[blockCount - 1]; //Get last block
    currentBlock.actions[currentBlock.actionsCount] = a;
    currentBlock.actionsCount++;
    buffer[blockCount - 1] = currentBlock;
}

/**
 * @brief Cancel action in the last block
 */
void cancelBlockAction(int i){
    block_t currentBlock = buffer[blockCount - 1]; //Get last block

    for(int n(0); n < currentBlock.actionsCount; n++){
        if(n >= i){
            currentBlock.actions[n] = currentBlock.actions[n-1];
        }
    }
}

/**
 * @brief Cancel action in a specific block
 */
void cancelAction(int blockIndex, int actionIndex){
    block_t currentBlock = buffer[blockIndex]; //Get last block

    for(int n(0); n < currentBlock.actionsCount; n++){
        if(n >= actionIndex){
            currentBlock.actions[n] = currentBlock.actions[n-1];
        }
    }
}

/**
 * @brief Create an empty block
 */
block_t emptyBlock(){
    block_t empty;
    empty.state = blockState::corrupted;
    empty.actionsCount = 0;
    empty.duration = 0;
    empty.id = -1;
    empty.scoreValue = 0;
    empty.cancelable = true;
    empty.index = 0;

    return empty;
}

/**
 * @brief Create an empty action
 */
action_t emptyActions(){
    action_t empty;
    empty.state = blockState::corrupted;
    empty.duration = 0;
    empty.id = -1;
    empty.scoreValue = 0;
    empty.cancelable = true;

    return empty;
}

/**
 * @brief Get a specific block
 */
block_t getBlock(int i){
    if(!(i >= 0  && i < blockCount)){
        return emptyBlock();
    }
    return buffer[i];
}

/**
 * @brief Get a specific block
 */
block_t getLastBlock(){
    return buffer[blockCount -1];
}

/**
 * @brief Get the current block
 */
block_t getCurrentBlock(){
    return buffer[blockIndex];
}

/**
 * @brief Get a specific action in the lastblock
 */
action_t getLastBlockAction(int i){
    block_t lastBlock = getLastBlock();
    if(!(i >= 0  && i < lastBlock.actionsCount)){
        return emptyActions();
    }
    return lastBlock.actions[i];
}

/**
 * @brief Get a specific action in a specific block
 */
action_t getAction(int blockIndex, int actionIndex){
    block_t currentBlock = getBlock(blockIndex);
    if(!(actionIndex >= 0  && actionIndex < currentBlock.actionsCount)){
        return emptyActions();
    }
    return currentBlock.actions[actionIndex];
}

/**
 * @brief Get the current block
 */
action_t getCurrentActions(){
    return buffer[blockIndex].actions[buffer[blockIndex].index];
}



/****************************
 *        Main logic        *
 ****************************/

void processActions(block_t cb, long timeLeft){
    //Process state
    action_t currentAction = cb.actions[cb.index];
    switch (currentAction.state)
    {
        case blockState::scheduled :
            if(timeLeft > currentAction.duration)
                execActions(currentAction);
            break;
        case blockState::started :
            checkAction(currentAction);
            break;
        case blockState::failed :
            handleFailure(currentAction);
            if(currentAction.cancelable){
                cb.index++;
                if(cb.index == cb.actionsCount) cb.state = blockState::done;
            }
            else cb.state = blockState::failed;
            break;
        case blockState::done :
            handleSuccess(currentAction);
            cb.index++;
            if(cb.index == cb.actionsCount) cb.state = blockState::done;
            break;      
        default:
            break;
    }
}

void processBlocks(long timeLeft){
    //Process state
    block_t currentBlock = getCurrentBlock();
    switch (currentBlock.state)
    {
        case blockState::scheduled :
            if(timeLeft > currentBlock.duration)
                execBlock(currentBlock);
            break;
        case blockState::started :
            processActions(currentBlock, timeLeft);
            break;
        case blockState::failed :
            handleBlockFailure(currentBlock);
            if(currentBlock.cancelable) blockIndex++;
            //else stuck
            break;
        case blockState::done :
            handleBlockSuccess(currentBlock);
            blockIndex++;
            if(blockIndex == blockCount) finMatch();
            break;      
        default:
            break;
    }
}


void checkBlock(block_t cb){

}

void checkAction(action_t cb){

}

void handleBlockFailure(block_t cb){

}

void handleBlockSuccess(block_t cb){

}

void handleFailure(action_t cb){

}

void handleSuccess(action_t cb){

}

/**
 * @brief Execute an action
 */
bool execActions(action_t a){
    switch (a.type)
    {
    case blockType::absMove :
        /* code */
        break;
    case blockType::relMove :
        turnGo(ADVERSAIRE_NON,false,true,0,180);
        break;
    case blockType::actuator :
        /* code */
        break;
    default:
        break;
    }
    return true;
}

/**
 * @brief Execute a Block
 */
bool execBlock(block_t a){

    return true;
}


/****************************
 *      Actions planner     *
 ****************************/
void planAbsoluteMove(float X, float Y){
    action_t a;
    a.cancelable = false;
    a.checkForOppenent = true;
    a.duration = -1; //To be calculated
    a.parameters[0] = X;
    a.parameters[1] = Y;
    a.scoreValue = 0;
    a.state = blockState::scheduled;
    a.type = blockType::absMove;
    planAction(a);
}

void planAbsoluteMove(float X, float Y, float alpha){
    action_t a;
    a.cancelable = false;
    a.checkForOppenent = true;
    a.duration = -1; //To be calculated
    a.parameters[0] = X;
    a.parameters[1] = Y;
    a.parameters[2] = alpha;
    a.scoreValue = 0;
    a.state = blockState::scheduled;
    a.type = blockType::absMove;
    planAction(a);
}

void planRelativeMove(float startAlpha, float distance){
   action_t a;
    a.cancelable = false;
    a.checkForOppenent = true;
    a.duration = -1; //To be calculated
    a.parameters[0] = startAlpha;
    a.parameters[1] = distance;
    a.scoreValue = 0;
    a.state = blockState::scheduled;
    a.type = blockType::absMove;
    planAction(a);
}

void planRelativeMove(float startAlpha, float distance, float endAlpha){
   action_t a;
    a.cancelable = false;
    a.checkForOppenent = true;
    a.duration = -1; //To be calculated
    a.parameters[0] = startAlpha;
    a.parameters[1] = distance;
    a.parameters[2] = endAlpha;
    a.scoreValue = 0;
    a.state = blockState::scheduled;
    a.type = blockType::absMove;
    planAction(a);
}


