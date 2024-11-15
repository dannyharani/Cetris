#include <stdio.h>
#include "raylib.h"

int main() {
    int window_height = 600;
    int window_width = 800;

    InitWindow(window_width, window_height, "Raylib C Tetris");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        {
            ClearBackground(RAYWHITE);
            DrawText("Tetris!", window_width/2, window_height/2, 42, GRAY);
        }
        EndDrawing();
    }

    CloseWindow(); 
    
    return 0;
}
