#ifndef ab_H // To make sure you don't declare the function more than once by including the header multiple times.
#define ab_H

#include <stdio.h>
#include <stdlib.h>
#include <string>
/* Color */
#define BLACK 1 /* player 1 */
#define WHITE 2 /* player 2 */
#define COLOR_SUM BLACK + WHITE
#define EMPTY -1
/* Direction */
#define UP 0
#define UP_RIGHT 1
#define RIGHT 2
#define DOWN_RIGHT 3
#define DOWN 4
#define DONW_LEFT 5
#define LEFT 6
#define UP_LEFT 7
/* border result */
#define notfound -1
/* numbers */
#define infinty 99
#define negative_infinity -99

int min_value(int current_depth, int target_dept, int alpha, int beta, int board[6][6], int player_color);

typedef struct grid
{
    int row;
    int column;
    int score;
    struct grid *next;
} Grid;

typedef struct step
{
    int row;
    int column;
} Step;

Grid *result;

void print_grid(Grid *grid)
{
    char row = grid->row + 65;
    char column = grid->column + 97;
    printf("%c%c\n", row, column);
}

string output_board(int board[6][6])
{
    string output = "";
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            int current_piece = board[i][j];
            if (current_piece == EMPTY)
            {
                output +='-';
            }
            else if (current_piece == BLACK)
            {
                output += 'B';
            }
            else if (current_piece == WHITE)
            {
                output += 'W';
            }
        }
    }
    return output;
}

int grid_in_board(Grid *grid)
{
    if (grid->row < 0 || grid->row >= 6)
    {
        return 0;
    }
    else if (grid->column < 0 || grid->column >= 6)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

/* find the same color on the direction so that
count_flipped_pieces() function know where to stop counting */
Grid *find_boundary(int board[6][6], int player_color, Grid *grid, Step *step)
{
    Grid *boundary = (Grid *)malloc(sizeof(Grid));
    Grid *current_grid = (Grid *)malloc(sizeof(Grid));
    current_grid->row = grid->row + step->row;
    current_grid->column = grid->column + step->column;
    int count = 0;
    while (grid_in_board(current_grid))
    {
        int row = current_grid->row;
        int column = current_grid->column;
        if (board[row][column] == player_color)
        {
            if (count == 0)
            {
                break;
            }
            boundary->row = row;
            boundary->column = column;
            return boundary;
        }
        else if (board[row][column] == EMPTY)
        {
            break;
        }
        else
        {
            count++;
        }
        current_grid->row += step->row;
        current_grid->column += step->column;
    }
    /* no piece with the same color */
    boundary->row = notfound;
    boundary->column = notfound;
    return boundary;
}

Step *get_step(int direction)
{
    Step *step = (Step *)malloc(sizeof(Step));
    ;
    if (direction == UP)
    {
        step->row = -1;
        step->column = 0;
    }
    else if (direction == UP_RIGHT)
    {
        step->row = -1;
        step->column = 1;
    }
    else if (direction == RIGHT)
    {
        step->row = 0;
        step->column = 1;
    }
    else if (direction == DOWN_RIGHT)
    {
        step->row = 1;
        step->column = 1;
    }
    else if (direction == DOWN)
    {
        step->row = 1;
        step->column = 0;
    }
    else if (direction == DONW_LEFT)
    {
        step->row = 1;
        step->column = -1;
    }
    else if (direction == LEFT)
    {
        step->row = 0;
        step->column = -1;
    }
    else if (direction == UP_LEFT)
    {
        step->row = -1;
        step->column = -1;
    }
    return step;
}

int count_flipped_pieces_in_direction(int board[6][6], int player_color, Grid *grid, int direction)
{
    Step *step = get_step(direction);
    int current_row = grid->row + step->row;
    int current_column = grid->column + step->column;
    Grid *boundary = find_boundary(board, player_color, grid, step);
    int flipped_pieces = 0;
    if (!(boundary->row == notfound && boundary->column == notfound))
    {
        while (current_row != boundary->row || current_column != boundary->column)
        {
            flipped_pieces++;
            current_row += step->row;
            current_column += step->column;
        }
    }
    free(step);
    free(boundary);
    return flipped_pieces;
}

void input_board(string board_str, int board[6][6])
{
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            char ch = board_str[i * 6 + j];
            if (ch == '-')
            {
                board[i][j] = EMPTY;
            }
            else if (ch == 'B')
            {
                board[i][j] = BLACK;
            }
            else if (ch == 'W')
            {
                board[i][j] = WHITE;
            }
        }
    }
}

int flippable(int board[6][6], int grid_row, int grid_column, int player_color)
{
    Grid *grid = (Grid *)malloc(sizeof(Grid));
    grid->row = grid_row;
    grid->column = grid_column;
    int sum_of_flipped_pieces = 0;
    for (int direction = 0; direction <= 7; direction++)
    {
        sum_of_flipped_pieces += count_flipped_pieces_in_direction(board, player_color, grid, direction);
    }
    free(grid);
    if (sum_of_flipped_pieces > 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/* heruistic score for each grid */
/* source: https://www.researchgate.net/figure/Position-value-of-each-cell-in-the-Othello-board_fig2_328458543*/
int grid_heuristic_score(int row, int column)
{
    int score[6][6] = {
        {50, -20, 5, 5, -20, 50},
        {-20, -50, -2, -2, -50, -20},
        {5, -2, -1, -1, -2, 5},
        {5, -2, -1, -1, -2, 5},
        {-20, -50, -2, -2, -50, -20},
        {50, -20, 5, 5, -20, 50}};
    return score[row][column];
}

Grid *find_dropable_grid(int board[6][6], int player_color, int min)
{
    Grid *first_dropable_grid = NULL;
    Grid *current_grid;
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            if (board[i][j] == EMPTY && flippable(board, i, j, player_color))
            {
                Grid *grid = (Grid *)malloc(sizeof(Grid));
                grid->row = i;
                grid->column = j;
                grid->next = NULL;
                grid->score = grid_heuristic_score(grid->row, grid->column);
                /* put dropable grid into linked list */
                if (first_dropable_grid == NULL)
                {
                    first_dropable_grid = grid;
                    current_grid = grid;
                }
                else
                {
                    if (min)
                    {
                        if (grid->score < first_dropable_grid->score)
                        {
                            grid->next = first_dropable_grid;
                            first_dropable_grid = grid;
                        }
                        else
                        {
                            Grid *prev = first_dropable_grid;
                            Grid *curr = first_dropable_grid->next;
                            while (curr != NULL && curr->score < grid->score)
                            {
                                prev = curr;
                                curr = curr->next;
                            }
                            prev->next = grid;
                            grid->next = curr;
                        }
                    }
                    else
                    {
                        if (grid->score > first_dropable_grid->score)
                        {
                            grid->next = first_dropable_grid;
                            first_dropable_grid = grid;
                        }
                        else
                        {
                            Grid *prev = first_dropable_grid;
                            Grid *curr = first_dropable_grid->next;
                            while (curr != NULL && curr->score > grid->score)
                            {
                                prev = curr;
                                curr = curr->next;
                            }
                            prev->next = grid;
                            grid->next = curr;
                        }
                    }
                }
            }
        }
    }
    return first_dropable_grid;
}

int count_color_pieces(int board[6][6], int player_color)
{
    int player_pieces = 0;
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            if (board[i][j] == player_color)
            {
                player_pieces++;
            }
        }
    }
    return player_pieces;
}

int heuristic_score(int board[6][6], int player_color)
{
    int opponent_color = COLOR_SUM - player_color;
    int score = count_color_pieces(board, player_color) - count_color_pieces(board, opponent_color);
    return score;
}

void flip_pieces(int board[6][6], int player_color, Grid *target_grid)
{
    if (!(target_grid->row == notfound && target_grid->column == notfound))
    {
        int row = target_grid->row;
        int column = target_grid->column;
        for (int direction = 0; direction <= 7; direction++)
        {
            Step *step = get_step(direction);
            int current_row = target_grid->row + step->row;
            int current_column = target_grid->column + step->column;
            Grid *boundary_grid = find_boundary(board, player_color, target_grid, step);
            if (!(boundary_grid->row == notfound && boundary_grid->column == notfound))
            {
                while (!(current_row == boundary_grid->row && current_column == boundary_grid->column))
                {
                    board[current_row][current_column] = player_color;
                    current_row += step->row;
                    current_column += step->column;
                }
            }
            free(step);
        }
        board[row][column] = player_color;
    }
}

void copy_board(int old_board[6][6], int new_board[6][6])
{
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            new_board[i][j] = old_board[i][j];
        }
    }
}

void free_grid_list(Grid *list_head)
{
    Grid *current = list_head;
    while (current != NULL)
    {
        Grid *next = current->next;
        free(current);
        current = next;
    }
}

int max_value(int current_depth, int target_dept, int alpha, int beta, int board[6][6], int player_color)
{
    if (current_depth == target_dept)
    {
        return heuristic_score(board, player_color);
    }
    else
    {
        Grid *grid_list = find_dropable_grid(board, player_color, 0);
        int max = negative_infinity;
        int opponent_color = COLOR_SUM - player_color;
        if (grid_list == NULL)
        {
            int new_board[6][6];
            copy_board(board, new_board);
            return min_value(current_depth + 1, target_dept, alpha, beta, new_board, opponent_color);
        }
        Grid *current_grid = grid_list;
        while (current_grid != NULL)
        {
            int new_board[6][6];
            copy_board(board, new_board);
            flip_pieces(new_board, player_color, current_grid);
            int score = min_value(current_depth + 1, target_dept, alpha, beta, new_board, opponent_color);
            if (score > max)
            {
                max = score;
                if (current_depth == 0)
                {
                    result->row = current_grid->row;
                    result->column = current_grid->column;
                }
            }
            if (max >= beta)
            {
                return max;
            }
            if (max > alpha)
            {
                alpha = max;
            }
            current_grid = current_grid->next;
        }
        free_grid_list(grid_list);
        return max;
    }
}

int min_value(int current_depth, int target_dept, int alpha, int beta, int board[6][6], int player_color)
{
    if (current_depth == target_dept)
    {
        return heuristic_score(board, player_color);
    }
    else
    {
        Grid *grid_list = find_dropable_grid(board, player_color, 1);
        int min = infinty;
        int opponent_color = COLOR_SUM - player_color;
        if (grid_list == NULL)
        {
            int new_board[6][6];
            copy_board(board, new_board);
            return max_value(current_depth + 1, target_dept, alpha, beta, new_board, opponent_color);
        }
        Grid *current_grid = grid_list;
        while (current_grid != NULL)
        {
            int new_board[6][6];
            copy_board(board, new_board);
            flip_pieces(new_board, player_color, current_grid);
            int score = max_value(current_depth + 1, target_dept, alpha, beta, new_board, opponent_color);
            if (score < min)
            {
                min = score;
            }
            if (min <= alpha)
            {
                return min;
            }
            if (min < beta)
            {
                beta = min;
            }
            current_grid = current_grid->next;
        }
        free_grid_list(grid_list);
        return min;
    }
}

void run_alpha_beta_pruning(int target_depth, int board[6][6], int player_color)
{
    int alpha = negative_infinity;
    int beta = infinty;
    int result_value = max_value(0, target_depth, alpha, beta, board, player_color);
    result_value++;
}

string alpha_beta_pruning(int player_color, int depth, string board_str)
{
    int board[6][6];
    input_board(board_str, board);

    result = (Grid *)malloc(sizeof(Grid));
    result->row = notfound;
    result->column = notfound;

    run_alpha_beta_pruning(depth, board, player_color);
    flip_pieces(board, player_color, result);
    return output_board(board);
}
#endif