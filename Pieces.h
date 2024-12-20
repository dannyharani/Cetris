#include "Coordinates.h"

typedef enum Piece_Type {
    O_TYPE,
    I_TYPE,
    S_TYPE,
    Z_TYPE,
    L_TYPE,
    J_TYPE,
    T_TYPE,
    NUM_PIECE_TYPES
} Piece_Type;

const Coordinate Piece_Coordinates[7][4][4] = 
{
    [O_TYPE] = {
        // Rotation
        {{0, 0}, {1, 0}, {0, 1}, {1, 1}},
        {{0, 0}, {1, 0}, {0, 1}, {1, 1}},
        {{0, 0}, {1, 0}, {0, 1}, {1, 1}},
        {{0, 0}, {1, 0}, {0, 1}, {1, 1}}
    },
    [I_TYPE] = {
        {{1, 0}, {1, 1}, {1, 2}, {1, 3}},
        {{0, 2}, {1, 2}, {2, 2}, {3, 2}},
        {{2, 0}, {2, 1}, {2, 2}, {2, 3}},
        {{0, 1}, {1, 1}, {2, 1}, {3, 1}}
    },
    [S_TYPE] = {
        {{0, 1}, {0, 2}, {1, 0}, {1, 1}},
        {{0, 1}, {1, 1}, {1, 2}, {2, 2}},
        {{1, 1}, {1, 2}, {2, 0}, {2, 1}},
        {{0, 0}, {1, 0}, {1, 1}, {2, 1}}
    },
    [Z_TYPE] = {
        {{0, 0}, {0, 1}, {1, 1}, {1, 2}},
        {{0, 2}, {1, 1}, {1, 2}, {2, 1}},
        {{1, 0}, {1, 1}, {2, 1}, {2, 2}},
        {{0, 1}, {1, 0}, {1, 1}, {2, 0}}
    },
    [L_TYPE] = {
        {{0, 2}, {1, 0}, {1, 1}, {1, 2}},
        {{0, 1}, {1, 1}, {2, 1}, {2, 2}},
        {{1, 0}, {1, 1}, {1, 2}, {2, 0}},
        {{0, 0}, {0, 1}, {1, 1}, {2, 1}}
    },
    [J_TYPE] = {
        {{0, 0}, {1, 0}, {1, 1}, {1, 2}},
        {{0, 1}, {0, 2}, {1, 1}, {2, 1}},
        {{1, 0}, {1, 1}, {1, 2}, {2, 2}},
        {{0, 1}, {1, 1}, {2, 0}, {2, 1}}
    },
    [T_TYPE] = {
        {{0, 1}, {1, 0}, {1, 1}, {1, 2}},
        {{0, 1}, {1, 1}, {1, 2}, {2, 1}},
        {{1, 0}, {1, 1}, {1, 2}, {2, 1}},
        {{0, 1}, {1, 0}, {1, 1}, {2, 1}}
    },
};

typedef struct Piece {
    Piece_Type  type;
    short       rotation;
    Coordinate  offset;
} Piece;