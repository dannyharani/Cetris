#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "raylib.h"
#include "Pieces.h"

/* 
* The game_grid holds the color of each game_piece. If the color is not blank {0, 0, 0, 0}, there is a piece there
* The current piece is not displayed on the grid until it becomes 'set'
* A piece's position is defined by a generic shape for the piece (with relation to its rotation) + its x and y offset (to be implemented)
*/

/* Definitions */
#define GAME_ROWS 20
#define GAME_COLS 10

#define ARRAY_SIZE(arr) ( sizeof(arr) / sizeof(arr[0]) )

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
void display_piece(int board_width, int board_height);

bool has_collided();
bool has_space_left();
bool has_space_right();
bool will_overlap();

void update_info_panel(int window_width, int window_height);

Piece get_piece();
void place_piece();

/* Globals */
Color game_grid[GAME_ROWS][GAME_COLS] = {0};
Piece current_piece;
Piece next_piece;

/* Project */
int main() {
    int window_width = 600;
    int window_height = 800;

    int frame;

    srand(time(NULL));

    InitWindow(window_width, window_height, "Raylib C Tetris");
    SetTargetFPS(60);
    current_piece = get_piece();
    next_piece = get_piece();

    while (!WindowShouldClose()) {
        BeginDrawing();
        frame++;
        
        ClearBackground(RAYWHITE);

        display_grid(window_width * 0.7, window_height);
        display_piece(window_width * 0.7, window_height);
        update_info_panel(window_width, window_height);

        if (frame % 30 == 0 || IsKeyPressed(KEY_DOWN)) {
            frame = 0;

            if (has_collided()) {
                place_piece();
                current_piece = next_piece;
                next_piece = get_piece();

            }

            current_piece.offset.row += 1;
        }

        if (IsKeyPressed(KEY_LEFT) && has_space_left()) {
            current_piece.offset.col--;
        }
        
        if (IsKeyPressed(KEY_RIGHT) && has_space_right()) {
            current_piece.offset.col++;
        }

        if (IsKeyPressed(KEY_UP) && !will_overlap()) {
            current_piece.rotation+=1;
            current_piece.rotation %= 4;
        }

        window_height = GetRenderHeight();
        window_width = GetRenderWidth();
        
        EndDrawing();
    }

    CloseWindow(); 
    
    return 0;
}

Piece get_piece() {
    Piece piece = (Piece){rand()%NUM_PIECE_TYPES, 0, {0, 0}};
    
    switch (piece.type) {
    case O_TYPE:
        piece.offset = (Coordinate){0,4};
        break;
    case I_TYPE:
        piece.offset = (Coordinate){-1,3};
        break;
    default:
        piece.offset = (Coordinate){0,4};
    }
    
    return piece;
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

void display_piece(int board_width, int board_height) {
    int box_width = board_width/GAME_COLS;
    int box_height = board_height/GAME_ROWS;

    Coordinate *cells = (Coordinate *)Piece_Coordinates[current_piece.type][current_piece.rotation];
    Color color = Piece_Color[current_piece.type];
    for (int i = 0; i < 4; i++) {
        int row_pos = cells[i].row + current_piece.offset.row;
        int col_pos = cells[i].col + current_piece.offset.col;
    
        DrawRectangle(col_pos * box_width, row_pos * box_height, box_width, box_height, color);
        DrawRectangleLines(col_pos * box_width, row_pos * box_height, box_width, box_height, BLACK);
    }
}

// Todo, rethink this

void update_info_panel(int window_width, int window_height) {
    int box_width = window_width * 0.7 * 0.8/GAME_COLS;
    int box_height = window_height * 0.8/GAME_ROWS;

    Coordinate *cells = (Coordinate *)Piece_Coordinates[next_piece.type][0];

    for (int i = 0; i < 4; i++) {
        int row_pos = cells[i].row;
        int col_pos = cells[i].col;

        DrawRectangle((col_pos * box_width) + (window_width * 0.7) + box_width*1.5, (row_pos+10) * box_height, box_width, box_height, Piece_Color[next_piece.type]);
        DrawRectangleLines((col_pos * box_width) + (window_width * 0.7 + box_width*1.5), (row_pos+10) * box_height, box_width, box_height, BLACK);
    }
}

bool has_collided() {

    Coordinate *cells = (Coordinate *) Piece_Coordinates[current_piece.type][current_piece.rotation];
    
    for (int i = 0; i < 4; i++) {
        int next_grid_row   = cells[i].row + current_piece.offset.row + 1;
        int grid_col        = cells[i].col + current_piece.offset.col;
        if (next_grid_row >= GAME_ROWS || game_grid[next_grid_row][grid_col].a != 0) {
            return true;
        }
    }

    return false;
}

bool has_space_left() {

    Coordinate *cells = (Coordinate *) Piece_Coordinates[current_piece.type][current_piece.rotation];
    
    for (int i = 0; i < 4; i++) {
        int grid_row        = cells[i].row + current_piece.offset.row;
        int next_grid_col   = cells[i].col + current_piece.offset.col - 1;
        if (next_grid_col < 0 || game_grid[grid_row][next_grid_col].a != 0) {
            return false;
        }
    }

    return true;
}

bool has_space_right() {

    Coordinate *cells = (Coordinate *) Piece_Coordinates[current_piece.type][current_piece.rotation];
    
    for (int i = 0; i < 4; i++) {
        int grid_row        = cells[i].row + current_piece.offset.row;
        int next_grid_col   = cells[i].col + current_piece.offset.col + 1;
        if (next_grid_col >= GAME_COLS || game_grid[grid_row][next_grid_col].a != 0) {
            return false;
        }
    }

    return true;
}

bool will_overlap() {

    Coordinate *cells = (Coordinate *) Piece_Coordinates[current_piece.type][(current_piece.rotation + 1) % 4];
    
    for (int i = 0; i < 4; i++) {
        int grid_row = cells[i].row + current_piece.offset.row;
        int grid_col = cells[i].col + current_piece.offset.col;
        if (grid_col >= GAME_COLS || grid_col < 0 || game_grid[grid_row][grid_col].a != 0) {
            return true;
        }
    }

    return false;
}

void place_piece() {
    for (int i = 0; i < 4; i++) {
        int row_pos = current_piece.offset.row + Piece_Coordinates[current_piece.type][current_piece.rotation][i].row;
        int col_pos = current_piece.offset.col + Piece_Coordinates[current_piece.type][current_piece.rotation][i].col;
        game_grid[row_pos][col_pos] = LIGHTGRAY;
    }
}