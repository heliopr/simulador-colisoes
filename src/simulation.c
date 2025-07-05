#include "simulation.h"

#include "raymath.h"
#include "cvector.h"

#include "ball.h"

Ball *balls;
BoundingBox box;
float boxSize = 30.0f;
int numBalls = 100;
float maxVelocity = 10.0f;
float minVelocity = 5.0f;

void simulation_Init()
{
    simulation_CalculateBoundingBox();
    simulation_CreateBalls();
}

void simulation_CreateBalls()
{
    float halfSize = boxSize/2;
    for (int i = 0; i < numBalls; i++)
    {
        Ball ball;
        ball.mass = ((float)GetRandomValue(50, 100))/10.0f;
        ball.radius = ball.mass/5.0f;
        ball.position = (Vector3){(float)GetRandomValue(-halfSize+ball.radius, halfSize-ball.radius), (float)GetRandomValue(-halfSize+ball.radius, halfSize-ball.radius), (float)GetRandomValue(-halfSize+ball.radius, halfSize-ball.radius)};
        ball.velocity = (Vector3){GetRandomValue(-100, 100), GetRandomValue(-100, 100), GetRandomValue(-100, 100)};
        ball.velocity = Vector3Scale(Vector3Normalize(ball.velocity), GetRandomValue(minVelocity, maxVelocity));
        ball.color = (Color){GetRandomValue(0, 255), GetRandomValue(0, 255), GetRandomValue(0, 255), 255};
        cvector_push_back(balls, ball);
    }
}

void simulation_Update(float dt)
{
    int numBalls = cvector_size(balls);
    for (int i = 0; i < numBalls; i++)
    {
        Ball *ball = &balls[i];
        Vector3 movement = ball->velocity;
        movement = Vector3Scale(movement, dt);
        ball->position = Vector3Add(ball->position, movement);
    }

    for (int i = 0; i < numBalls; i++)
    {
        Ball *a = &balls[i];
        for (int j = i+1; j < numBalls; j++)
        {
            Ball *b = &balls[j];
            simulation_HandleCollision(a, b);
        }

        simulation_HandleBoxCollision(a);
    }
}

void simulation_HandleCollision(Ball *a, Ball *b)
{
    Vector3 delta = Vector3Subtract(b->position, a->position);
    float distance = Vector3Length(delta);
    float minDistance = a->radius + b->radius;

    if (distance == 0.0f)
        return;

    if (distance < minDistance)
    {
        float penetration = minDistance - distance;
        Vector3 n = Vector3Scale(delta, 1.0f/distance);
        Vector3 correction = Vector3Scale(n, penetration / 2.0f);
        a->position = Vector3Subtract(a->position, correction);
        b->position = Vector3Add(b->position, correction);

        float v1n = Vector3DotProduct(a->velocity, n);
        float v2n = Vector3DotProduct(b->velocity, n);
        float m1 = a->mass, m2 = b->mass;
        float v1nFinal = ((m1-m2)*v1n + 2*m2*v2n) / (m1+m2);
        float v2nFinal = ((m2-m1)*v2n + 2*m1*v1n) / (m1+m2);

        a->velocity = Vector3Add(a->velocity, Vector3Scale(n, v1nFinal-v1n));
        b->velocity = Vector3Add(b->velocity, Vector3Scale(n, v2nFinal-v2n));
    }
}

void simulation_CalculateBoundingBox()
{
    float halfSize = boxSize/2;
    box = (BoundingBox){{-halfSize, -halfSize, -halfSize}, {halfSize, halfSize, halfSize}};
}

float simulation_CalculateKinecticEnergy()
{
    float k = 0.0f;

    for (int i = 0; i < cvector_size(balls); i++)
    {
        Ball ball = balls[i];
        float v = Vector3Length(ball.velocity);
        k += ball.mass * v*v * (1.0f/2);
    }

    return k;
}

void simulation_HandleBoxCollision(Ball *ball)
{
    Vector3 boxMin = box.min;
    Vector3 boxMax = box.max;

    float radius = ball->radius;
    boxMin.x += radius;
    boxMin.y += radius;
    boxMin.z += radius;
    boxMax.x -= radius;
    boxMax.y -= radius;
    boxMax.z -= radius;

    Vector3 position = ball->position;
    Vector3 velocity = ball->velocity;

    // ISSO É UM CRIME MAS FUNCIONA (KISS)
    if (position.x < boxMin.x)
    {
        position.x = boxMin.x;
        velocity.x = -velocity.x;
    } 
    else if (position.x > boxMax.x)
    {
        position.x = boxMax.x;
        velocity.x = -velocity.x;
    }

    if (position.y < boxMin.y)
    {
        position.y = boxMin.y;
        velocity.y = -velocity.y;
    } 
    else if (position.y > boxMax.y)
    {
        position.y = boxMax.y;
        velocity.y = -velocity.y;
    }

    if (position.z < boxMin.z)
    {
        position.z = boxMin.z;
        velocity.z = -velocity.z;
    } 
    else if (position.z > boxMax.z)
    {
        position.z = boxMax.z;
        velocity.z = -velocity.z;
    }

    ball->position = position;
    ball->velocity = velocity;
}

void simulation_UpdateConfig(int numBallsValue, float boxSizeValue, float minVelocityValue, float maxVelocityValue)
{
    if (boxSizeValue != boxSize)
    {
        boxSize = boxSizeValue;
        simulation_CalculateBoundingBox();
    }

    if (numBallsValue != numBalls)
    {
        numBalls = numBallsValue;
    }

    if (minVelocityValue != minVelocity)
    {
        minVelocity = minVelocityValue;
    }

    if (maxVelocityValue != maxVelocity)
    {
        maxVelocity = maxVelocityValue;
    }

    cvector_clear(balls);
    simulation_CreateBalls();
}

BoundingBox simulation_GetBox()
{
    return box;
}

float simulation_GetBoxSize()
{
    return boxSize;
}

void simulation_End()
{
    cvector_free(balls);
}

Ball *simulation_GetBalls()
{
    return balls;
}