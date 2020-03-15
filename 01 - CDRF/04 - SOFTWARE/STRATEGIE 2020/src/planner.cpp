#include "planner.h"

void planAction(action_t a){

}

void cancelAction(int i);
action_t getAction(int i);

void planAbsoluteMove(float X, float Y);
void planAbsoluteMove(float X, float Y, float alpha);

void planRelativeMove(float startAlpha, float distance);
void planRelativeMove(float startAlpha, float distance, float endAlpha);

void processActions(long timeLeft);
