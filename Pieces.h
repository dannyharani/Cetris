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

extern const Coordinate Piece_Coordinates[7][4][4];

typedef struct Piece {
    Piece_Type  type;
    short       rotation;
    Coordinate  offset;
} Piece;