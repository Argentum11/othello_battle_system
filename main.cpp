#include "MonteCarlo.hpp"
#include "AlphaBetaPruning.hpp"
#include "Mix.hpp"

// strategy
#define ALPHA_BETA_PRUNING 1
#define MONTE_CARLO 2
#define MIX 3

string run_strategy(int strategy, int player, int depth, int iteration, string board)
{
    if (strategy == ALPHA_BETA_PRUNING)
    {
        char *board_str = new char[board.size() + 1]; // Allocate memory for the null-terminated string
        strcpy(board_str, board.c_str());
        return alpha_beta_pruning(player, depth, board_str);
    }
    else if (strategy == MONTE_CARLO)
    {
        return monte_carlo(player, iteration, board);
    }
    else
    {
        return mix(player, depth, iteration, board);
    }
}

int winner_index(string board_str, int main_board[SIZE][SIZE])
{
    input_board(board_str, main_board);
    int black_score = heuristic_score(main_board, BLACK);
    int white_score = heuristic_score(main_board, WHITE);
    if (black_score > white_score)
    {
        return 0; // Black goes first
    }
    else
    {
        return 1;
    }
}

void display_result(int strategy[2], int win[2], string detail[2])
{
    string strategy_name[4] = {"",
                               "alpha beta pruning",
                               "monte carlo",
                               "mix"};
    int winner = 0;
    for (int i = 0; i < 2; i++)
    {
        cout << strategy_name[strategy[i]] << detail[i] << " wins " << win[i] << " games" << endl;
        if (win[i] > win[winner])
        {
            winner = i;
        }
    }
    cout << strategy_name[strategy[winner]] << detail[winner] << " wins !!!" << endl;
}

void swap(int array[2])
{
    int temp = array[0];
    array[0] = array[1];
    array[1] = temp;
}

void swap(string array[2])
{
    string temp = array[0];
    array[0] = array[1];
    array[1] = temp;
}

void swap_strategy(int strategy[2], int input_depth[2], int input_iteration[2], string strategy_detail[2], int win[2])
{
    swap(strategy);
    swap(input_depth);
    swap(input_iteration);
    swap(strategy_detail);
    swap(win);
}

int main()
{
    string order[2] = {"first", "second"};
    int strategy[2], input_depth[2], input_iteration[2], color[2];
    color[0] = BLACK;
    color[1] = WHITE;
    cout << endl;
    for (int i = 0; i < 2; i++)
    {
        cout << "Please select " << order[i] << " strategy, alpha beta pruning...1/monte carlo...2/mix...3/: ";
        cin >> strategy[i];
        if (strategy[i] == ALPHA_BETA_PRUNING)
        {
            cout << "Please enter depth: ";
            cin >> input_depth[i];
        }
        else if (strategy[i] == MONTE_CARLO)
        {
            cout << "Please enter iteration: ";
            cin >> input_iteration[i];
        }
        else if (strategy[i] == MIX)
        {
            cout << "Please enter depth: ";
            cin >> input_depth[i];
            cout << "Please enter iteration: ";
            cin >> input_iteration[i];
        }
    }
    string strategy_detail[2];
    for (int i = 0; i < 2; i++)
    {
        if (strategy[i] == ALPHA_BETA_PRUNING)
        {
            strategy_detail[i] = to_string(input_depth[i]);
        }
        else if (strategy[i] == MONTE_CARLO)
        {
            strategy_detail[i] = to_string(input_iteration[i]);
        }
        else
        {
            strategy_detail[i] = to_string(input_depth[i]) + "," + to_string(input_iteration[i]);
        }
        strategy_detail[i] = "(" + strategy_detail[i] + ")";
    }
    int win[2];
    for (int i = 0; i < 2; i++)
    {
        win[i] = 0;
    }

    int main_board[6][6];
    for (int game = 1; game <= 3; game++)
    {
        cout << "Game " << game << endl;
        if (game == 2)
        {
            swap_strategy(strategy, input_depth, input_iteration, strategy_detail, win);
        }
        for (int i = 0; i < SIZE; i++)
        {
            for (int j = 0; j < SIZE; j++)
            {
                main_board[i][j] = EMPTY;
            }
        }
        main_board[SIZE / 2 - 1][SIZE / 2 - 1] = WHITE;
        main_board[SIZE / 2][SIZE / 2] = WHITE;
        main_board[SIZE / 2 - 1][SIZE / 2] = BLACK;
        main_board[SIZE / 2][SIZE / 2 - 1] = BLACK;
        string board_str = output_board(main_board);
        // two strategy battle
        int index = 0, same = 0;
        string prev_board_str = "";
        while (same != 2)
        {
            board_str = run_strategy(strategy[index], color[index], input_depth[index], input_iteration[index], board_str);
            index = 1 - index;
            if (board_str == prev_board_str)
            {
                same++;
            }
            else
            {
                same = 0;
            }
            prev_board_str = board_str;
            // cout << board_str << endl;
            cout << ".";
        }
        cout << endl;
        win[winner_index(board_str, main_board)]++;
        if (game == 2)
        {
            swap_strategy(strategy, input_depth, input_iteration, strategy_detail, win);
        }
    }
    display_result(strategy, win, strategy_detail);
}