#ifndef CONSTANTS_H
#define CONSTANTS_H


// CHANGE: collision types
enum CollisionType {
    circle_ = 0,
    ray_ = 1
};

enum EnemyMoveState {
    state_patrol = 0,
    state_intercept = 1,
    state_chase = 2
};

#endif
