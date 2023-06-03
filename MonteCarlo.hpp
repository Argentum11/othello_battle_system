#ifndef maco_H    // To make sure you don't declare the function more than once by including the header multiple times.
#define maco_H

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>

using namespace std;

const int SIZE = 6;

enum Piece
{
    EMPTY,
    BLACK,
    WHITE
};

Piece board[SIZE][SIZE];
Piece player = BLACK;
int check_end = 0;

int countPieces(Piece piece)
{
    int count = 0;
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            if (board[i][j] == piece)
            {
                count++;
            }
        }
    }
    return count;
}

void copyBoard(Piece dest[SIZE][SIZE], Piece src[SIZE][SIZE])
{
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            dest[i][j] = src[i][j];
        }
    }
}

bool isOnBoard(int x, int y)
{
    return x >= 0 && x < SIZE && y >= 0 && y < SIZE;
}

bool isValidMove(int x, int y)
{
    if (!isOnBoard(x, y) || board[x][y] != EMPTY)
    {
        return false;
    }
    int dx[] = {-1, -1, 0, 1, 1, 1, 0, -1};
    int dy[] = {0, 1, 1, 1, 0, -1, -1, -1};
    bool isValid = false;
    for (int i = 0; i < 8; i++)
    {
        int tx = x + dx[i];
        int ty = y + dy[i];
        if (isOnBoard(tx, ty) && board[tx][ty] != EMPTY && board[tx][ty] != player)
        {
            while (isOnBoard(tx, ty) && board[tx][ty] != EMPTY && board[tx][ty] != player)
            {
                tx += dx[i];
                ty += dy[i];
            }
            if (isOnBoard(tx, ty) && board[tx][ty] == player)
            {
                isValid = true;
            }
        }
    }
    return isValid;
}

void makeMove(int x, int y)
{
    board[x][y] = player;
    int dx[] = {-1, -1, 0, 1, 1, 1, 0, -1};
    int dy[] = {0, 1, 1, 1, 0, -1, -1, -1};
    for (int i = 0; i < 8; i++)
    {
        int tx = x + dx[i];
        int ty = y + dy[i];
        if (isOnBoard(tx, ty) && board[tx][ty] != EMPTY && board[tx][ty] != player)
        {
            while (isOnBoard(tx, ty) && board[tx][ty] != EMPTY && board[tx][ty] != player)
            {
                tx += dx[i];
                ty += dy[i];
            }
            if (isOnBoard(tx, ty) && board[tx][ty] == player)
            {
                int sx = x + dx[i];
                int sy = y + dy[i];
                while (isOnBoard(sx, sy) && board[sx][sy] != EMPTY && board[sx][sy] != player)
                {
                    board[sx][sy] = player;
                    sx += dx[i];
                    sy += dy[i];
                }
            }
        }
    }
}

string printBoard()
{
    string output = "";
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            if (board[i][j] == EMPTY)
            {
                output += "-";
            }
            else if (board[i][j] == BLACK)
            {
                output += "B";
            }
            else if (board[i][j] == WHITE)
            {
                output += "W";
            }
        }
    }
    return output;
}

vector<pair<int, int>> getValidMoves()
{
    vector<pair<int, int>> validMoves;
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            if (isValidMove(i, j))
            {
                validMoves.push_back(make_pair(i, j));
            }
        }
    }
    return validMoves;
}

bool gameOver(Piece pl)
{
    Piece plcopy = pl;
    vector<pair<int, int>> validMoves = getValidMoves();

    // cout << "validMoves.size() = " << validMoves.size() << '\n';
    if (validMoves.size() == 0)
    {
        player = (player == BLACK) ? WHITE : BLACK;
        validMoves = getValidMoves();
        if (validMoves.size() == 0)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    player = plcopy;
    return false;
}

void randomGame()
{
    while (true)
    {
        if (gameOver(player))
        {
            break;
        }
        vector<pair<int, int>> validMoves = getValidMoves();
        if (validMoves.size() > 0)
        {
            int moveIndex = rand() % validMoves.size();
            makeMove(validMoves[moveIndex].first, validMoves[moveIndex].second);
            player = (player == BLACK) ? WHITE : BLACK;
        }
        else
        {
            player = (player == BLACK) ? WHITE : BLACK;
        }
    }
}

double getScore()
{
    int blackCount = countPieces(BLACK);
    int whiteCount = countPieces(WHITE);

    if (blackCount > whiteCount)
    {
        return 1.0;
    }
    else if (blackCount < whiteCount)
    {
        return 0.0;
    }
    else
    {
        return 0.5;
    }
}

double runMonteCarlo(int iterations)
{
    int wins = 0;
    for (int i = 0; i < iterations; i++)
    {
        Piece boardCopy[SIZE][SIZE];
        copyBoard(boardCopy, board);
        Piece playerCopy = player;
        randomGame();
        double score = getScore();
        if (score == 1.0 && playerCopy == BLACK || score == 0.0 && playerCopy == WHITE)
        {
            wins++;
        }
        copyBoard(board, boardCopy);
        player = playerCopy;
    }
    return (double)wins / (double)iterations;
}

string monte_carlo(int player_number, int iterations, string input)
{
    srand(time(0));
    /*board[SIZE / 2 - 1][SIZE / 2 - 1] = WHITE;
    board[SIZE / 2][SIZE / 2] = WHITE;
    board[SIZE / 2 - 1][SIZE / 2] = BLACK;
    board[SIZE / 2][SIZE / 2 - 1] = BLACK;
    player = BLACK;*/
    // int player_number;
    // cin >> player_number;
    if (player_number == 1)
    {
        player = BLACK;
    }
    else
    {
        player = WHITE;
    }
    // int iterations;
    // cin >> iterations;
    // string input;
    // cin >> input;
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            char input_piece = input[i * SIZE + j];
            if (input_piece == '-')
            {
                board[i][j] = EMPTY;
            }
            else if (input_piece == 'B')
            {
                board[i][j] = BLACK;
            }
            else
            {
                board[i][j] = WHITE;
            }
        }
    }
    // printBoard();

    /*while (true)
    {
        printBoard();

        if (gameOver(player))
        {
            cout << "end" << '\n';
            break;
        }

        if (player == BLACK)
        {
            vector<pair<int, int>> validMoves = getValidMoves();
            if (validMoves.size() > 0)
            {
                int moveIndex = rand() % validMoves.size();
                makeMove(validMoves[moveIndex].first, validMoves[moveIndex].second);
            }

            player = WHITE;
        }
        else
        {*/
    double bestScore = -1.0;
    int bestX = -1;
    int bestY = -1;
    vector<pair<int, int>> validMoves = getValidMoves();
    for (int i = 0; i < validMoves.size(); i++)
    {
        int x = validMoves[i].first;
        int y = validMoves[i].second;
        board[x][y] = player;
        double score = runMonteCarlo(iterations);
        if (score > bestScore)
        {
            bestScore = score;
            bestX = x;
            bestY = y;
        }
        board[x][y] = EMPTY;
    }
    if (bestX != -1 && bestY != -1)
    {
        makeMove(bestX, bestY);
    }
    return printBoard();

    /*player = BLACK;
}
}*/
}

#endif