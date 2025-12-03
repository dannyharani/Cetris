#pragma once

#include "raylib.h"
#include "Pieces.h"
#include "ai.h"

/* Definitions */
#define GAME_ROWS 20
#define GAME_COLS 10

#define ARRAY_SIZE(arr) ( sizeof(arr) )

extern Color Piece_Color[];

typedef struct GameState {
    bool            cell_occupied[GAME_ROWS][GAME_COLS];
    Piece           current_piece;
    Piece           next_piece;
    int             score;
    bool            lost;

    GameHeuristics  GameHeuristics;

    Piece           piece_bag[NUM_PIECE_TYPES];
    int             pieces_in_bag;
} GameState;

typedef Color RenderGrid[GAME_ROWS][GAME_COLS];

typedef enum MOVE {
    NONE = 0,

    RIGHT,
    LEFT,

    ROTATE_CW,
    ROTATE_CCW,

    DOWN,
    HARD_DOWN    
} MOVE;

/* 
* The game_grid holds the color of each game_piece. If the color is not blank {0, 0, 0, 0}, there is a piece there
* The current piece is not displayed on the grid until it becomes 'set'
* A piece's position is defined by a generic shape for the piece (with relation to its rotation) + its x and y offset (to be implemented)
*/
void game_loop(GameState *state, RenderGrid *render_grid, int window_widht, int window_height, int *frame);

void display_grid(RenderGrid *render_grid, int board_width, int board_height);
void display_piece(GameState *state, int board_width, int board_height);

bool has_collided(GameState *state);
bool has_space_left(GameState *state);
bool has_space_right(GameState *state);
bool will_overlap(GameState *state);
void update_lose_state(GameState *state);
void check_grid(GameState *state, RenderGrid *render_grid);
void clear_row(GameState *state, RenderGrid *render_grid, int row);
void clear_grid(GameState *state, RenderGrid *render_grid);
int get_shadow_row(GameState *state);

void shift_grid_from_clear();

void update_info_panel(GameState *state, int panel_width, int panel_height, int x_offset);

Piece get_piece(GameState *state);
void place_piece(GameState *state, RenderGrid *render_grid);