#pragma once

#include "raylib.h"
#include "Pieces.h"

/* Definitions */
#define GAME_ROWS 20
#define GAME_COLS 10

#define ARRAY_SIZE(arr) ( sizeof(arr) )
#define ARRAY_LEN(arr) (sizeof(arr) / sizeof(arr)[0])

extern Color Piece_Color[];

typedef struct GameState {
    bool            cell_occupied[GAME_ROWS][GAME_COLS];
    Piece           current_piece;
    Piece           next_piece;
    int             score;
    bool            lost;

    Piece           piece_bag[NUM_PIECE_TYPES];
    int             pieces_in_bag;
} GameState;

typedef enum MOVE {
    NONE = 0,

    RIGHT,
    LEFT,

    ROTATE_CW,
    ROTATE_CCW,

    DOWN,
    HARD_DOWN    
} MOVE;

Piece get_piece(GameState *state);