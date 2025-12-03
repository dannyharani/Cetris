#include "ai.h"
#include "main.h"
#include <stdlib.h>
#include <math.h>

GameState *ai_games = NULL;
int ai_game_count = 0;

void get_heuristics(GameState *state) {
    /*
     * What should we consider a hole?
     * is the following 1 hole, or 3:
     * xxxx
     * x--x
     * x-xx
     * xxxx
     */

    int num_holes = 0;
    int agg_height = 0;
    int heights[GAME_COLS] = {0};
    int bumpiness = 0;

    for (int col = 0; col < GAME_COLS; col++) {

        bool ceiling_exists = false;
        int col_height = 0;

        for (int row = 0; row < GAME_ROWS; row++) {

            if (state->cell_occupied[row][col] && !ceiling_exists) {
                ceiling_exists = true;
                col_height = GAME_ROWS - row;
            } else if (ceiling_exists && !state->cell_occupied[row][col]) {
                num_holes++;
            }
        }
        heights[col] = col_height;
        agg_height += col_height;
    }

    for (int col = 0; col < GAME_COLS-1; col++) {
        bumpiness += abs(heights[col] - heights[col+1]);
    }

    state->GameHeuristics.aggregate_heights = agg_height;
    state->GameHeuristics.bumpiness = bumpiness;
    state->GameHeuristics.holes = num_holes;
}

void instantiate_games(int count) {
    if (ai_games != NULL) {
        free(ai_games);
    }

    ai_game_count = count;
    ai_games = (GameState *)malloc(sizeof(GameState) * count);

    for (int i = 0; i < count; i++) {
        // Initialize game state
        GameState *game = &ai_games[i];
        
        // Zero out the struct
        *game = (GameState){0};
        
        // Initialize specific fields as in main.c
        // cell_occupied is 0
        // current_piece, next_piece need initialization
        // score 0, lost false
        // piece_bag needs init? get_piece handles bag init if empty.
        
        game->pieces_in_bag = 0; // get_piece will refill
        
        game->current_piece = get_piece(game);
        game->next_piece = get_piece(game);
    }
}
