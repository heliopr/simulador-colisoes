#ifndef BALL_H
#define BALL_H

#include "raylib.h"

typedef struct {
    Vector3 position;
    Vector3 velocity;
    float radius;
    float mass;
    Color color;
} Ball;

#endif