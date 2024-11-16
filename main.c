#include <stdio.h>
#include <string.h>
#include "raylib.h"

/* 
* The game_grid holds the color of each game_piece. If the color is not blank {0, 0, 0, 0}, there is a piece there
* The current piece is not displayed on the grid until it becomes 'set'
* A piece's position is defined by a generic shape for the piece (with relation to its rotation) + its x and y offset (to be implemented)
*/

/* Definitions */
#define GAME_ROWS 20
#define GAME_COLS 10

#define O_COLOR (Color){0, 255, 255, 255};

#define ARRAY_SIZE(arr) ( sizeof(arr) / sizeof(arr[0]) )

Color game_grid[GAME_ROWS][GAME_COLS] = {0};

typedef struct Coordinate {
    int row;
    int col;
} Coordinate;

typedef enum Piece_Type {
    O_TYPE,
    I_TYPE,
    S_TYPE,
    Z_TYPE,
    L_TYPE,
    J_TYPE,
    T_TYPE
} Piece_Type;

typedef enum Direction {
    LEFT,
    RIGHT,
    DOWN
} Direction;

typedef struct Piece {
    Piece_Type  type;
    short       rotation;
    Coordinate  position;
    Coordinate  offset;
} Piece;

Color Piece_Color[] = {
    [O_TYPE] = (Color){0,   255,    255,   255},
    [I_TYPE] = (Color){255, 255,    0,     255},
    [S_TYPE] = (Color){255, 0,      255,   255},
    [Z_TYPE] = (Color){255, 0,      0,     255},
    [L_TYPE] = (Color){0,   255,    0,     255},
    [J_TYPE] = (Color){0,   0,      255,   255},
    [T_TYPE] = (Color){50,  50,     50,    255}
};

void display_grid(int board_width, int board_height);
void shift_grid(int row, int *frame);

Piece get_piece(Piece_Type piece_type);
void place_piece(Piece piece);

/* Project */
int main() {
    int window_width = 600;
    int window_height = 800;

    int frame;

    InitWindow(window_width, window_height, "Raylib C Tetris");
    SetTargetFPS(60);
    Piece piece = get_piece(O_TYPE);
    place_piece(piece);

    while (!WindowShouldClose()) {
        BeginDrawing();
        {
            frame++;
            ClearBackground(RAYWHITE);

            display_grid(window_width * 0.7, window_height);

            window_height = GetRenderHeight();
            window_width = GetRenderWidth();
        }
        EndDrawing();
    }

    CloseWindow(); 
    
    return 0;
}

void display_grid(int board_width, int board_height) {
    int box_width = board_width/GAME_COLS;
    int box_height = board_height/GAME_ROWS;

    for (int row = 0; row < GAME_ROWS; ++row) {
        for (int col = 0; col < GAME_COLS; ++col) {
            DrawRectangle(col * box_width, row * box_height, box_width, box_height, game_grid[row][col]);
            DrawRectangleLines(col * box_width, row * box_height, box_width, box_height, BLACK);
        }
    }
}

void shift_grid(int row, int *frame) {

    if (*frame % 30 != 0) {
        return;
    }

    for (int i = row-1; i > 0; i--) {
        memcpy(game_grid[i], game_grid[i-1], sizeof(game_grid[0]));
    }

    memset(game_grid[0], 0, sizeof(game_grid[0]));

    *frame = 0;
}

Piece get_piece(Piece_Type piece_type) {
    Piece piece = (Piece){O_TYPE, 0};

    switch (piece_type) {
    case O_TYPE:
        // piece.positions[0] = (Coordinate){0, 0};
        // piece.positions[1] = (Coordinate){0, 1};
        // piece.positions[2] = (Coordinate){1, 0};
        // piece.positions[3] = (Coordinate){1, 1};
        break;
    default:
        break;
    }
    return piece;
}

void place_piece(Piece piece) {
    for (int i = 0; i < 4; i++) {
        // game_grid[piece.positions[i].row][piece.positions[i].col] = Piece_Color[piece.type];
    }
}