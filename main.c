#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

Color Piece_Color[] = {
    [O_TYPE] = (Color){0,   255,    255,   255},
    [I_TYPE] = (Color){255, 255,    0,     255},
    [S_TYPE] = (Color){255, 0,      255,   255},
    [Z_TYPE] = (Color){255, 0,      0,     255},
    [L_TYPE] = (Color){0,   255,    0,     255},
    [J_TYPE] = (Color){0,   0,      255,   255},
    [T_TYPE] = (Color){50,  50,     50,    255}
};

typedef Color RenderGrid[GAME_ROWS][GAME_COLS];

void game_loop(GameState *state, RenderGrid *render_grid, int window_widht, int window_height, int *frame);
void display_grid(RenderGrid *render_grid, int board_width, int board_height);
void display_piece(GameState *state, int board_width, int board_height);
bool has_collided(GameState *state);
bool has_space_left(GameState *state);
bool has_space_right(GameState *state);
bool will_overlap(GameState *state);
void update_lose_state(GameState *state);
int check_grid(GameState *state, RenderGrid *render_grid);
void clear_row(GameState *state, RenderGrid *render_grid, int row);
void clear_grid(GameState *state, RenderGrid *render_grid);
int get_shadow_row(GameState *state);
void shift_grid_from_clear();
void update_info_panel(GameState *state, int panel_width, int panel_height, int x_offset);
void place_piece(GameState *state, RenderGrid *render_grid);


int FRAMES_TO_ACTIVATE_SPEED = 20;
int FRAMES_HELD_DOWN = 0;

/* Project */
int main() {
    int window_width = 600;
    int window_height = 800;

    int frame;

    srand(time(NULL));

    InitWindow(window_width, window_height, "Raylib C Tetris");
    SetTargetFPS(60);

    GameState game_state = {
        .cell_occupied = {{}},
        .current_piece = {0},
        .next_piece = {0},
        .score = 0,
        .lost = false,
        .piece_bag = {},
        .pieces_in_bag = 0
    };

    game_state.current_piece = get_piece(&game_state);
    game_state.next_piece = get_piece(&game_state);

    RenderGrid render_grid;
    memset(&render_grid, 0, ARRAY_SIZE(render_grid));

    while (!WindowShouldClose()) {
        BeginDrawing();
        frame++;

        game_loop(&game_state, &render_grid, window_width, window_height, &frame);

        window_height = GetRenderHeight();
        window_width = GetRenderWidth();

        EndDrawing();
    }

    CloseWindow();

    return 0;
}

void draw_frame(GameState *state, RenderGrid *render_grid, int window_width, int window_height) {
    ClearBackground(RAYWHITE);
    update_info_panel(state, window_width * 0.3, window_height, window_width * 0.7);

    if (!state->lost) {
        display_grid(render_grid, window_width * 0.7, window_height);
        display_piece(state, window_width * 0.7, window_height);
    } else {
        DrawText("You lost!\nPress anywhere\nto continue.", window_width/4, window_height/3, 40, BLACK);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            clear_grid(state, render_grid);
            state->score = 0;
            state->lost = false;
        }
    }
}

void make_move(GameState *state, RenderGrid *render_grid, MOVE move) {
    switch (move) {
        case HARD_DOWN:
            if (!has_collided(state)) {
                state->current_piece.offset.row = get_shadow_row(state);
            }

            place_piece(state, render_grid);
            state->current_piece = state->next_piece;

            update_lose_state(state);
            if (state->lost)
                return;

            state->next_piece = get_piece(state); // To be from bag of all seven pieces no replacement
            check_grid(state, render_grid);
            break;

        case DOWN:
            if (has_collided(state)) {
                place_piece(state, render_grid);
                state->current_piece = state->next_piece;

                update_lose_state(state);
                if (state->lost)
                    return;

                state->next_piece = get_piece(state);
                check_grid(state, render_grid);
            } else {
                state->current_piece.offset.row += 1;
            }
            break;

        case LEFT:
            state->current_piece.offset.col--;
            break;

        case RIGHT:
            state->current_piece.offset.col++;
            break;

        case ROTATE_CW:
            state->current_piece.rotation+=1;
            state->current_piece.rotation%=4;
            break;

        case ROTATE_CCW:
            state->current_piece.rotation-=1;
            state->current_piece.rotation%=4;
        case NONE:
            break;
    }
}

void game_loop(GameState *state, RenderGrid *render_grid, int window_width, int window_height, int *frame) {
    draw_frame(state, render_grid, window_width, window_height);
    int gravity = 30;

    if (!state->lost) {

        if (IsKeyDown(KEY_DOWN)) {
            if (FRAMES_HELD_DOWN >= FRAMES_TO_ACTIVATE_SPEED) {
                gravity = 4;
            } else {
                FRAMES_HELD_DOWN++;
            }
        }

        if (IsKeyReleased(KEY_DOWN)) {
            FRAMES_HELD_DOWN = 0;
        }

        if (*frame >= gravity || IsKeyPressed(KEY_DOWN)){
            *frame = 0;
            make_move(state, render_grid, DOWN);
        }

        if (IsKeyPressed(KEY_LEFT) && has_space_left(state)) {
            make_move(state, render_grid, LEFT);
        }

        if (IsKeyPressed(KEY_RIGHT) && has_space_right(state)) {
            make_move(state, render_grid, RIGHT);
        }

        if (IsKeyPressed(KEY_UP) && !will_overlap(state)) {
            make_move(state, render_grid, ROTATE_CW);
        }
    }
}

Piece get_piece(GameState *state) {
    int a = rand()%NUM_PIECE_TYPES;
    int b = rand()%NUM_PIECE_TYPES;

    Piece piece_a = state->piece_bag[a];
    Piece piece_b = state->piece_bag[b];

    if (state->pieces_in_bag == 0) {
        for (int i = 0; i < NUM_PIECE_TYPES; i++) {
            state->piece_bag[i] = (Piece){i, 0, {0, 0}};
        }

        for (int i = 0; i < 14; i++) {
            a = rand()%NUM_PIECE_TYPES;
            b = rand()%NUM_PIECE_TYPES;

            piece_a = state->piece_bag[a];
            piece_b = state->piece_bag[b];
            state->piece_bag[a] = piece_b;
            state->piece_bag[b] = piece_a;
        }

        state->pieces_in_bag = NUM_PIECE_TYPES;
    }

    Piece piece = state->piece_bag[NUM_PIECE_TYPES - state->pieces_in_bag];
    state->pieces_in_bag--;

    switch (piece.type) {
    case O_TYPE:
        piece.offset = (Coordinate){-1,4};
        break;
    case I_TYPE:
        piece.offset = (Coordinate){-2,3};
        break;
    default:
        piece.offset = (Coordinate){-1,4};
    }

    return piece;
}

void display_grid(RenderGrid *render_grid, int board_width, int board_height) {
    int box_width = board_width/GAME_COLS;
    int box_height = board_height/GAME_ROWS;

    for (int row = 0; row < GAME_ROWS; ++row) {
        for (int col = 0; col < GAME_COLS; ++col) {
            DrawRectangle(col * box_width, row * box_height, box_width, box_height, (*render_grid)[row][col]);
            DrawRectangleLines(col * box_width, row * box_height, box_width, box_height, BLACK);
        }
    }
}

void display_piece(GameState *state, int board_width, int board_height) {
    int box_width = board_width/GAME_COLS;
    int box_height = board_height/GAME_ROWS;

    Coordinate *cells = (Coordinate *)Piece_Coordinates[state->current_piece.type][state->current_piece.rotation];
    Color color = Piece_Color[state->current_piece.type];
    for (int i = 0; i < 4; i++) {
        int row_pos = cells[i].row + state->current_piece.offset.row;
        int col_pos = cells[i].col + state->current_piece.offset.col;

        DrawRectangle(col_pos * box_width, row_pos * box_height, box_width, box_height, color);
        DrawRectangleLines(col_pos * box_width, row_pos * box_height, box_width, box_height, BLACK);
    }
}

void update_info_panel(GameState *state, int panel_width, int panel_height, int x_offset) {
    int grid_height = panel_height/GAME_ROWS;
    int grid_width = panel_width/GAME_COLS;

    char score_str[10];
    sprintf(score_str, "%d", state->score);

    DrawText("Score:", x_offset + grid_width*2, grid_height*4, 35, BLUE);
    DrawText(score_str, x_offset + grid_width*3, grid_height*5, 40, BLUE);

    // Drawing the next piece
    DrawText("Next Piece", x_offset + grid_width, grid_height*16, 25, BLUE);
    Coordinate *cells = (Coordinate *)Piece_Coordinates[state->next_piece.type][0];
    for (int i = 0; i < 4; i++) {
        int row_pos = cells[i].row;
        int col_pos = cells[i].col;

        DrawRectangle(x_offset + (grid_width * (col_pos + state->next_piece.offset.col)), (grid_width * row_pos) + (grid_height * 17), grid_width, grid_width, Piece_Color[state->next_piece.type]);
        DrawRectangleLines(x_offset + (grid_width * (col_pos + state->next_piece.offset.col)), (grid_width * row_pos) + (grid_height * 17), grid_width, grid_width, BLACK);

    }
}

bool has_collided(GameState *state) {

    Coordinate *cells = (Coordinate *) Piece_Coordinates[state->current_piece.type][state->current_piece.rotation];

    for (int i = 0; i < 4; i++) {
        int next_grid_row   = cells[i].row + state->current_piece.offset.row + 1;
        int grid_col        = cells[i].col + state->current_piece.offset.col;
        if (next_grid_row >= GAME_ROWS || state->cell_occupied[next_grid_row][grid_col]) {
            return true;
        }
    }

    return false;
}

bool has_space_left(GameState *state) {

    Coordinate *cells = (Coordinate *) Piece_Coordinates[state->current_piece.type][state->current_piece.rotation];

    for (int i = 0; i < 4; i++) {
        int grid_row        = cells[i].row + state->current_piece.offset.row;
        int next_grid_col   = cells[i].col + state->current_piece.offset.col - 1;

        if (next_grid_col < 0) return false;

        if (grid_row >= 0 && grid_row < GAME_ROWS) {
            if (state->cell_occupied[grid_row][next_grid_col]) {
                return false;
            }
        }
    }

    return true;
}

bool has_space_right(GameState *state) {
    Coordinate *cells = (Coordinate *) Piece_Coordinates[state->current_piece.type][state->current_piece.rotation];

    for (int i = 0; i < 4; i++) {
        int grid_row        = cells[i].row + state->current_piece.offset.row;
        int next_grid_col   = cells[i].col + state->current_piece.offset.col + 1;

        if (next_grid_col >= GAME_COLS) return false;

        if (grid_row >= 0 && grid_row < GAME_ROWS) {
            if (state->cell_occupied[grid_row][next_grid_col]) {
                return false;
            }
        }
    }

    return true;
}

bool will_overlap(GameState *state) {
    Coordinate *cells = (Coordinate *) Piece_Coordinates[state->current_piece.type][(state->current_piece.rotation + 1) % 4];

    for (int i = 0; i < 4; i++) {
        int grid_row = cells[i].row + state->current_piece.offset.row;
        int grid_col = cells[i].col + state->current_piece.offset.col;

        if (grid_col < 0 || grid_col >= GAME_COLS) return true;

        if (grid_row >= 0 && grid_row < GAME_ROWS) {
            if (state->cell_occupied[grid_row][grid_col]) {
                return true;
            }
        }
    }

    return false;
}

void update_lose_state(GameState *state) {
    Coordinate *cells = (Coordinate *) Piece_Coordinates[state->current_piece.type][(state->current_piece.rotation)];

    for (int i = 0; i < 4; i++) {
        int grid_row = cells[i].row + state->current_piece.offset.row;
        int grid_col = cells[i].col + state->current_piece.offset.col;

        // int state_row = state->current_piece.offset.row;
        // int state_col = state->current_piece.offset.col;

        // printf("[Block %d] Target: [%d, %d] | Piece Anchor: [%d, %d] | Content: %s\n",
        //     i,
        //     grid_row,
        //     grid_col,
        //     state_row,
        //     state_col,
        //     (grid_row >= 0 && grid_row < GAME_ROWS && grid_col >= 0 && grid_col < GAME_COLS)
        //         ? (state->cell_occupied[grid_row][grid_col] ? "OCCUPIED" : "EMPTY")
        //         : "OUT OF BOUNDS"
        // );

        if (grid_col < 0 || grid_col >= GAME_COLS || grid_row >= GAME_ROWS || (grid_row >= 0 && state->cell_occupied[grid_row][grid_col])) {

            state->lost = true;
            break;
        }
    }
}

int get_shadow_row(GameState *state) {
    int orginal_row = state->current_piece.offset.row;

    while (!has_collided(state)) {
        state->current_piece.offset.row++;
    }

    int landing_row = state->current_piece.offset.row;
    state->current_piece.offset.row = orginal_row;

    return landing_row;
}

void place_piece(GameState *state, RenderGrid *render_grid) {
    for (int i = 0; i < 4; i++) {
        int row_pos = state->current_piece.offset.row + Piece_Coordinates[state->current_piece.type][state->current_piece.rotation][i].row;
        int col_pos = state->current_piece.offset.col + Piece_Coordinates[state->current_piece.type][state->current_piece.rotation][i].col;

        if (row_pos >= 0 && row_pos < GAME_ROWS && col_pos >= 0 && col_pos < GAME_COLS) {
            state->cell_occupied[row_pos][col_pos] = true;
            if (render_grid) {
                (*render_grid)[row_pos][col_pos] = LIGHTGRAY;
            }
        }
    }
}

int check_grid(GameState *state, RenderGrid *render_grid) {
    int rows_cleared = 0;

    for (int row = GAME_ROWS - 1; row >= 0 ; row--) {
        bool row_is_full = true;
        for (int col = 0; col < GAME_COLS; col++) {
            if (state->cell_occupied[row][col] == 0){
                row_is_full = false;
                break;
            }
        }

        if (row_is_full) {
            rows_cleared++;

            for (int clear_row_i = row; clear_row_i > 0; clear_row_i--) {
                memcpy(state->cell_occupied[clear_row_i], state->cell_occupied[clear_row_i - 1], sizeof(state->cell_occupied[0]));
                if (render_grid){
                    memcpy((*render_grid)[clear_row_i], (*render_grid)[clear_row_i - 1], sizeof(*render_grid[0]));
                }
            }

            clear_row(state, render_grid, 0);
            row++; // Recheck current row as we pulled down rows from above
            state->score+=100;
        }
    }

    return rows_cleared;
}

void clear_row(GameState *state, RenderGrid *render_grid, int row) {
    memset(state->cell_occupied[row], 0, sizeof(state->cell_occupied[row]));
    if (render_grid) {
        memset((*render_grid)[row], 0, sizeof((*render_grid)[row]));
    }
}

void clear_grid(GameState *state, RenderGrid *render_grid) {
    for (int row = 0; row < GAME_ROWS; row++) {
        clear_row(state, render_grid, row);
    }
}
