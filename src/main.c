#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include <math.h>

#include "simulation.h"
#include "renderer.h"

void DrawSphereBasic(Color color);

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 600;

	SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(screenWidth, screenHeight, "Simulador de Colisoes 3D");

    SetTargetFPS(60);

	renderer_Init();
	simulation_Init();

	int showMessageBox = false;
    while (!WindowShouldClose())
    {
		float dt = GetFrameTime();
		// UPDATE
		simulation_Update(dt);
        render_Update();

        BeginDrawing();
        ClearBackground(BLACK);

		// RENDER 3D
		renderer_Render3D();

		// RENDER GUI
		renderer_RenderGUI();
		DrawFPS(10, 10);

        EndDrawing();
    }

	simulation_End();

    CloseWindow();

    return 0;
}