#include "primlib.h"
#include "puzzles.h"
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define BLOCK_SIZE 30.0
#define GRID_WIDTH 10
#define GRID_HEIGHT 20
#define GRID_WIDTH_START (gfx_screenWidth() - GRID_WIDTH * BLOCK_SIZE) / 2
#define GRID_HEIGHT_START gfx_screenHeight() - GRID_HEIGHT *BLOCK_SIZE
#define PUZZLE_WIDTH 4
#define PUZZLE_HEIGHT 4
#define ROTATIONS 4
#define START_PUZZLE_POS_X GRID_WIDTH_START + 4 * BLOCK_SIZE
#define START_PUZZLE_POS_Y GRID_HEIGHT_START
#define START_MOVE_X 4
#define NEXT_PUZZLE_POS_X gfx_screenWidth() * 3 / 4
#define NEXT_PUZZLE_POS_Y GRID_HEIGHT_START

int board[GRID_WIDTH][GRID_HEIGHT];
int current_puzzle[ROTATIONS][PUZZLE_WIDTH][PUZZLE_HEIGHT];
int next_puzzle[ROTATIONS][PUZZLE_WIDTH][PUZZLE_HEIGHT];
int move_x = 0;
int move_y = 0;
int rotation = 0;
int main_puzzle_x = 0;
int main_puzzle_y = 0;
int rand_puzzle = 0;
int next_rand_puzzle = 0;

int check_rotation()
{
    rotation = rotation % 4;
    return rotation;
}

void clear_screen()
{
    gfx_filledRect(0, 0, gfx_screenWidth(), gfx_screenHeight(), BLACK);
}

void initial_gamestate()
{
    for (int grid_x = 0; grid_x < GRID_WIDTH; grid_x++)
    {
        for (int grid_y = 0; grid_y < GRID_HEIGHT; grid_y++)
        {
            board[grid_x][grid_y] = 0;
        }
    }
}

void draw_grid()
{
    for (int grid_x = 0; grid_x < GRID_WIDTH; grid_x++)
    {
        for (int grid_y = 0; grid_y < GRID_HEIGHT; grid_y++)
        {
            gfx_rect(GRID_WIDTH_START + grid_x * BLOCK_SIZE, GRID_HEIGHT_START + grid_y * BLOCK_SIZE,
                     GRID_WIDTH_START + (grid_x + 1) * BLOCK_SIZE, GRID_HEIGHT_START + (grid_y + 1) * BLOCK_SIZE, WHITE);
        }
    }
}

void pick_puzzle(int rand_puzzle)
{
    for (int rotation = 0; rotation < ROTATIONS; rotation++)
    {
        for (int puzzle_x = 0; puzzle_x < PUZZLE_WIDTH; puzzle_x++)
        {
            for (int puzzle_y = 0; puzzle_y < PUZZLE_HEIGHT; puzzle_y++)
            {
                current_puzzle[rotation][puzzle_x][puzzle_y] = pieces[rand_puzzle][rotation][puzzle_x][puzzle_y];
            }
        }
    }
}

void pick_next_puzzle(int rand_puzzle)
{
    for (int rotation = 0; rotation < ROTATIONS; rotation++)
    {
        for (int puzzle_x = 0; puzzle_x < PUZZLE_WIDTH; puzzle_x++)
        {
            for (int puzzle_y = 0; puzzle_y < PUZZLE_HEIGHT; puzzle_y++)
            {
                next_puzzle[rotation][puzzle_x][puzzle_y] = pieces[rand_puzzle][rotation][puzzle_x][puzzle_y];
            }
        }
    }
}

void initial_puzzle()
{
    rand_puzzle = rand() % 7;
    next_rand_puzzle = rand() % 7;
    while (rand_puzzle == next_rand_puzzle)
    {
        next_rand_puzzle = rand() % 7;
    }
    pick_puzzle(rand_puzzle);
    pick_next_puzzle(next_rand_puzzle);
}

void new_puzzle()
{
    rand_puzzle = next_rand_puzzle;
    pick_puzzle(rand_puzzle);
    next_rand_puzzle = rand() % 7;
    pick_next_puzzle(next_rand_puzzle);
    move_x = 0;
    move_y = 0;
    rotation = 0;
}

void draw_next_puzzle(int color)
{
    for (int puzzle_x = 0; puzzle_x < PUZZLE_WIDTH; puzzle_x++)
    {
        for (int puzzle_y = 0; puzzle_y < PUZZLE_HEIGHT; puzzle_y++)
        {
            if (next_puzzle[0][puzzle_x][puzzle_y] == 2)
            {
                gfx_filledRect(NEXT_PUZZLE_POS_X + (puzzle_x)*BLOCK_SIZE, NEXT_PUZZLE_POS_Y + (puzzle_y)*BLOCK_SIZE,
                               NEXT_PUZZLE_POS_X + (puzzle_x + 1) * BLOCK_SIZE, NEXT_PUZZLE_POS_Y + (puzzle_y + 1) * BLOCK_SIZE, color + 1);
            }
            else if (next_puzzle[0][puzzle_x][puzzle_y] == 1)
            {
                gfx_filledRect(NEXT_PUZZLE_POS_X + (puzzle_x)*BLOCK_SIZE, NEXT_PUZZLE_POS_Y + (puzzle_y)*BLOCK_SIZE,
                               NEXT_PUZZLE_POS_X + (puzzle_x + 1) * BLOCK_SIZE, NEXT_PUZZLE_POS_Y + (puzzle_y + 1) * BLOCK_SIZE, color);
            }
        }
    }
}

void draw_puzzle(int color)
{
    for (int puzzle_x = 0; puzzle_x < PUZZLE_WIDTH; puzzle_x++)
    {
        for (int puzzle_y = 0; puzzle_y < PUZZLE_HEIGHT; puzzle_y++)
        {
            if (current_puzzle[rotation][puzzle_x][puzzle_y] == 2)
            {
                gfx_filledRect(START_PUZZLE_POS_X + (puzzle_x + move_x) * BLOCK_SIZE, START_PUZZLE_POS_Y + (puzzle_y + move_y) * BLOCK_SIZE,
                               START_PUZZLE_POS_X + (puzzle_x + move_x + 1) * BLOCK_SIZE, START_PUZZLE_POS_Y + (puzzle_y + move_y + 1) * BLOCK_SIZE, color + 1);
                main_puzzle_x = puzzle_x;
                main_puzzle_y = puzzle_y;
            }
            else if (current_puzzle[rotation][puzzle_x][puzzle_y] == 1)
            {
                gfx_filledRect(START_PUZZLE_POS_X + (puzzle_x + move_x) * BLOCK_SIZE, START_PUZZLE_POS_Y + (puzzle_y + move_y) * BLOCK_SIZE,
                               START_PUZZLE_POS_X + (puzzle_x + move_x + 1) * BLOCK_SIZE, START_PUZZLE_POS_Y + (puzzle_y + move_y + 1) * BLOCK_SIZE, color);
            }
        }
    }
}

void draw_board()
{
    for (int grid_x = 0; grid_x < GRID_WIDTH; grid_x++)
    {
        for (int grid_y = 0; grid_y < GRID_HEIGHT; grid_y++)
        {
            if (board[grid_x][grid_y] == 1)
            {
                gfx_filledRect(GRID_WIDTH_START + grid_x * BLOCK_SIZE, GRID_HEIGHT_START + grid_y * BLOCK_SIZE,
                               GRID_WIDTH_START + (grid_x + 1) * BLOCK_SIZE, GRID_HEIGHT_START + (grid_y + 1) * BLOCK_SIZE, YELLOW);
            }
        }
    }
}

void add_to_board()
{
    for (int puzzle_x = 0; puzzle_x < PUZZLE_WIDTH; puzzle_x++)
    {
        for (int puzzle_y = 0; puzzle_y < PUZZLE_HEIGHT; puzzle_y++)
        {
            int shift_x = move_x + START_MOVE_X;
            if (current_puzzle[rotation][puzzle_x][puzzle_y] == 1)
            {
                if (move_y + puzzle_y == GRID_HEIGHT || board[shift_x + puzzle_x][move_y + puzzle_y] == 1)
                {
                    for (int x = 0; x < PUZZLE_WIDTH; x++)
                    {
                        for (int y = 0; y < PUZZLE_HEIGHT; y++)
                        {
                            if (current_puzzle[rotation][x][y] != 0 && (move_y + y) >= 1)
                            {
                                board[shift_x + x][move_y - 1 + y] = 1;
                            }
                        }
                    }
                    new_puzzle();
                    return;
                }
            }
        }
    }
}

void check_for_borders()
{
    for (int puzzle_x = 0; puzzle_x < PUZZLE_WIDTH; puzzle_x++)
    {
        for (int puzzle_y = 0; puzzle_y < PUZZLE_HEIGHT; puzzle_y++)
        {
            int shift_x = move_x + START_MOVE_X;
            if (current_puzzle[rotation][puzzle_x][puzzle_y] != 0)
            {
                if (shift_x + puzzle_x == GRID_WIDTH)
                {
                    move_x--;
                }
                else if (shift_x + puzzle_x <= -1)
                {
                    move_x++;
                }
                else if (board[shift_x + puzzle_x][move_y + puzzle_y] == 1)
                {
                    move_x++;
                }
            }
        }
    }
}

void check_for_collision_left()
{
    move_x++;
    for (int puzzle_x = 0; puzzle_x < PUZZLE_WIDTH; puzzle_x++)
    {
        for (int puzzle_y = 0; puzzle_y < PUZZLE_HEIGHT; puzzle_y++)
        {
            if (current_puzzle[rotation][puzzle_x][puzzle_y] != 0)
            {
                int shift_x = move_x + START_MOVE_X;
                if (shift_x + puzzle_x == GRID_WIDTH)
                {
                    move_x--;
                }
                else if (board[move_x + START_MOVE_X + puzzle_x][move_y + puzzle_y] != 0)
                {
                    move_x--;
                }
            }
        }
    }
}

void clear_full_rows()
{
    for (int grid_y = 0; grid_y < GRID_HEIGHT; grid_y++)
    {
        int row_sum = 0;
        for (int grid_x = 0; grid_x < GRID_WIDTH; grid_x++)
        {
            row_sum += board[grid_x][grid_y];
        }
        if (row_sum == GRID_WIDTH)
        {
            for (int i = grid_y; i > 0; i--)
            {
                for (int grid_x = 0; grid_x < GRID_WIDTH; grid_x++)
                {
                    board[grid_x][i] = board[grid_x][i - 1];
                }
            }
            for (int grid_x = 0; grid_x < GRID_WIDTH; grid_x++)
            {
                board[grid_x][0] = 0;
            }
        }
    }
}
void check_space()
{
    int temp_move_x = move_x;
    int temp_move_y = move_y;
    int prev_rotation = rotation;
    rotation++;
    rotation = check_rotation(rotation);
    for (int puzzle_x = 0; puzzle_x < PUZZLE_WIDTH; puzzle_x++)
    {
        for (int puzzle_y = 0; puzzle_y < PUZZLE_HEIGHT; puzzle_y++)
        {
            if (current_puzzle[rotation][puzzle_x][puzzle_y] == 2)
            {
                move_x += main_puzzle_x - puzzle_x;
                move_y += main_puzzle_y - puzzle_y;
            }
        }
    }
    for (int puzzle_x = 0; puzzle_x < PUZZLE_WIDTH; puzzle_x++)
    {
        for (int puzzle_y = 0; puzzle_y < PUZZLE_WIDTH; puzzle_y++)
        {
            if (current_puzzle[rotation][puzzle_x][puzzle_y] != 0)
            {
                if (move_x + START_MOVE_X < 0)
                {
                    rotation = prev_rotation;
                    move_x = temp_move_x;
                    move_y = temp_move_y;
                }
                else if (move_x + puzzle_x + START_MOVE_X >= GRID_WIDTH)
                {
                    rotation = prev_rotation;
                    move_x = temp_move_x;
                    move_y = temp_move_y;
                }
                else if (move_y < 0)
                {
                    rotation = prev_rotation;
                    move_x = temp_move_x;
                    move_y = temp_move_y;
                }
                else if (move_y + puzzle_y > GRID_HEIGHT)
                {
                    rotation = prev_rotation;
                    move_x = temp_move_x;
                    move_y = temp_move_y;
                }
                else if (board[move_x + START_MOVE_X + puzzle_x][move_y + puzzle_y] != 0)
                {
                    rotation = prev_rotation;
                    move_x = temp_move_x;
                    move_y = temp_move_y;
                }
            }
        }
    }
}

void game_over()
{
    int row_sum = 0;
    for (int grid_x = 0; grid_x < GRID_WIDTH; grid_x++)
    {
        row_sum += board[grid_x][0];
    }
    if (row_sum > 0)
    {
        SDL_Delay(1000);
        clear_screen();
        gfx_textout(gfx_screenWidth() / 2, gfx_screenHeight() / 2, "GAME OVER", YELLOW);
        gfx_updateScreen();
        SDL_Delay(3000);
        initial_gamestate();
        new_puzzle();
    }
}

int main()
{

    if (gfx_init())
    {
        exit(3);
    }

    srand(time(NULL));
    int time = 0;

    initial_gamestate();
    initial_puzzle();
    printf("%d %d", rand_puzzle, next_rand_puzzle);

    while (1)
    {
        clear_full_rows();
        clear_screen();

        draw_board();
        draw_puzzle(rand_puzzle);
        draw_next_puzzle(next_rand_puzzle);

        draw_grid();
        game_over();
        add_to_board();

        int keycode = gfx_pollkey();
        if (keycode == SDLK_RIGHT)
        {
            check_for_collision_left();
            check_for_borders();
            printf("x - %d\n", move_x);
            printf("y - %d\n", move_y);
        }
        else if (keycode == SDLK_LEFT)
        {
            move_x--;
            check_for_borders();
            printf("x - %d\n", move_x);
            printf("y - %d\n", move_y);
        }
        else if (keycode == SDLK_DOWN)
        {
            move_y++;
            time = 0;
        }
        else if (keycode == SDLK_UP)
        {
            check_space();
            printf("x - %d\n", move_x);
            printf("y - %d\n", move_y);
        }

        gfx_updateScreen();

        if (time == 2000)
        {
            move_y++;
            time = 0;
        }
        time++;
    }
}