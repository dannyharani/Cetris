#pragma once
#include "main.h"

typedef struct GameHeuristics {
    int aggregate_heights;
    int holes;
    int bumpiness;
    int lines_cleared;
} GameHeuristics;

typedef struct AiGameState {
    GameState game;
    GameHeuristics heuristics;
} AiGameState;

void get_heuristics(AiGameState *state);
void instantiate_games(int count);
