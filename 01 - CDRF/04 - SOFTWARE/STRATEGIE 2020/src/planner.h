#ifndef PLANNER_H
#define PLANNER_H


enum BlockType { servo, absoluteMove, relativeMove };

struct block_t {
    // Data members
    BlockType blockType;
};













#endif