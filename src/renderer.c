#include "renderer.h"

#include "raylib.h"
#include "raymath.h"
#include "cvector.h"

#include "simulation.h"
#include "ball.h"

Camera camera = {0};

bool configWindowActive = true;
bool numBallsEditMode = false;
bool boxSizeEditMode = false;
bool minVelocityEditMode = false;
bool maxVelocityEditMode = false;
int numBallsValue = 100;
int boxSizeValue = 30;
int minVelocityValue = 5;
int maxVelocityValue = 10;

void renderer_Init()
{
    camera.position = (Vector3){ 40.0f, 10.0f, 40.0f };
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;
}

void renderer_Update()
{
    UpdateCamera(&camera, CAMERA_ORBITAL);
}

void renderer_RenderGUI()
{
    float kinecticEnergy = simulation_CalculateKinecticEnergy();
    DrawText(TextFormat("Energia cinética: %f", kinecticEnergy), 100, 10, 20, RAYWHITE);
    DrawText(TextFormat("Numero de bolinhas: %d", cvector_size(simulation_GetBalls())), 10, 30, 20, RAYWHITE);

    if (configWindowActive)
    {
        configWindowActive = !GuiWindowBox((Rectangle){ 10, 200, 280, 168 }, "CONFIGURAÇÃO");
        if (GuiValueBox((Rectangle){ 130, 230, 120, 24 }, "NUMERO BOLINHAS", &numBallsValue, numBallsValue, 10000, numBallsEditMode)) numBallsEditMode = !numBallsEditMode;
        if (GuiValueBox((Rectangle){ 130, 260, 120, 24 }, "TAMANHO CAIXA", &boxSizeValue, boxSizeValue, 10000, boxSizeEditMode)) boxSizeEditMode = !boxSizeEditMode;
        if (GuiValueBox((Rectangle){ 130, 290, 120, 24 }, "VELOCIDADE MINIMA", &minVelocityValue, minVelocityValue, 10000, minVelocityEditMode)) minVelocityEditMode = !minVelocityEditMode;
        if (GuiValueBox((Rectangle){ 130, 320, 120, 24 }, "VELOCIDADE MAXIMA", &maxVelocityValue, maxVelocityValue, 10000, maxVelocityEditMode)) maxVelocityEditMode = !maxVelocityEditMode;
    }

    if (GuiButton((Rectangle){ 10, 60, 120, 24 }, "#05#CONFIG"))
    {
        configWindowActive = true;
    }

    if (GuiButton((Rectangle){ 140, 60, 120, 24 }, "#74#ATUALIZAR"))
    {
        simulation_UpdateConfig(numBallsValue, (float)boxSizeValue, (float)minVelocityValue, (float)maxVelocityValue);
    }
}

void renderer_Render3D()
{
    BeginMode3D(camera);

    float boxSize = simulation_GetBoxSize();
    DrawCubeWires((Vector3){0.0f, 0.0f, 0.0f}, boxSize, boxSize, boxSize, WHITE);

    Ball *balls = simulation_GetBalls();
    for (int i = 0; i < cvector_size(balls); i++)
    {
        Ball ball = balls[i];
        DrawSphere(ball.position, ball.radius, ball.color);
    }

    EndMode3D();
}