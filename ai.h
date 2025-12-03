#pragma once

typedef struct GameHeuristics {
    int aggregate_heights;
    int holes;
    int bumpiness;
    int lines_cleared;
} GameHeuristics;

struct GameState;
void get_heuristics(struct GameState *state);
void instantiate_games(int count);
