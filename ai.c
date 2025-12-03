#include "ai.h"
#include "main.h"
#include <stdlib.h>

AiGameState *ai_games = NULL;
int ai_game_count = 0;

void get_heuristics(AiGameState *state) {
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

            if (state->game.cell_occupied[row][col] && !ceiling_exists) {
                ceiling_exists = true;
                col_height = GAME_ROWS - row;
            } else if (ceiling_exists && !state->game.cell_occupied[row][col]) {
                num_holes++;
            }
        }
        heights[col] = col_height;
        agg_height += col_height;
    }

    for (int col = 0; col < GAME_COLS-1; col++) {
        bumpiness += abs(heights[col] - heights[col+1]);
    }

    state->heuristics.aggregate_heights = agg_height;
    state->heuristics.bumpiness = bumpiness;
    state->heuristics.holes = num_holes;
}

void instantiate_games(int count) {
    if (ai_games != NULL) {
        free(ai_games);
    }

    ai_game_count = count;
    ai_games = (AiGameState *)malloc(sizeof(AiGameState) * count);

    for (int i = 0; i < count; i++) {
        AiGameState *state = &ai_games[i];

        *state = (AiGameState){0};

        state->game.current_piece = get_piece(&state->game);
        state->game.next_piece = get_piece(&state->game);
    }
}
