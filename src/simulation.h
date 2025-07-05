#ifndef SIMULATION_H
#define SIMULATION_H

#include "ball.h"

void simulation_Init();
void simulation_Update(float dt);
void simulation_CreateBalls();
void simulation_CalculateBoundingBox();
void simulation_HandleCollision(Ball *a, Ball *b);
float simulation_CalculateKinecticEnergy();
void simulation_HandleBoxCollision(Ball *ball);
BoundingBox simulation_GetBox();
float simulation_GetBoxSize();
void simulation_UpdateConfig(int numBallsValue, float boxSizeValue, float minVelocityValue, float maxVelocityValue);
void simulation_End();
Ball *simulation_GetBalls();

#endif