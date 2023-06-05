#ifndef MIXHUE
#define MIXHUE
#include <bits/stdc++.h>
#include "MixFun.hpp"
using namespace std;
#define SIZE 6
string mixhue_ret;
int mixhue_minimaxSearch(string gameboard, int originalplayer, int player, int depth, int d, int alpha, int beta, int iteration);

int mixhue_minimaxSearch(string gameboard, int originalplayer, int player, int depth, int d, int alpha, int beta, int iterations)
{
    // if (getValidMoves(gameboard, player).size() == 0)
    // {
    //     mixhue_ret = gameboard;
    //     return 0;
    // }
    // cout << gameboard << ' ' << d << endl;
    player--;
    if (d == depth)
    {
        // return countColorPieces(gameboard, originalplayer) - countColorPieces(gameboard, (!(originalplayer - 1)) + 1);
        int wins = 0;
        for (int i = 0; i < iterations; i++)
        {
            gameboard = randomGame(gameboard, player + 1);
            double score = getScore(gameboard);
            if (score == 1.0 && originalplayer == 1 || score == 0.0 && originalplayer == 2)
            {
                wins++;
            }
        }
        return wins;
    }
    char board[SIZE][SIZE];
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            board[i][j] = gameboard[i * SIZE + j];
        }
    }
    pair<int, string> temp = {0, ""};
    string pos = "";
    bool canmove = false;
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            bool flag = false;
            pos = 'A' + i;
            pos += 'a' + j;
            for (int dir = 0; dir < 8; dir++)
            {
                if (countFlipPieces(gameboard, player + 1, pos, dir))
                {
                    flag = true;
                    canmove = true;
                    break;
                }
            }
            if (flag)
            {
                int val = mixhue_minimaxSearch(flipPieces(gameboard, player + 1, pos), originalplayer, (!player) + 1, depth, d + 1, alpha, beta, iterations);
                if (temp.second == "")
                {
                    temp.first = val;
                    temp.second = pos;
                    continue;
                }

                if (player == originalplayer - 1)
                {
                    alpha = max(alpha, val);
                    if (temp.first < val)
                    {
                        temp.first = val;
                        temp.second = pos;
                    }
                    else if (temp.first == val && rand() % 2)
                    {
                        temp.first = val;
                        temp.second = pos;
                    }
                    if (beta <= alpha)
                    {
                        return temp.first;
                    }
                }
                else
                {
                    beta = min(beta, val);
                    if (temp.first > val)
                    {
                        temp.first = val;
                        temp.second = pos;
                    }
                    else if (temp.first == val && rand() % 2)
                    {
                        temp.first = val;
                        temp.second = pos;
                    }
                    if (beta <= alpha)
                    {
                        return temp.first;
                    }
                }
            }
        }
    }
    if (!canmove)
    {
        temp = {mixhue_minimaxSearch(gameboard, originalplayer, (!player) + 1, depth, d + 1, alpha, beta, iterations), ""};
    }
    if (d == 0)
    {
        // cout << player<<endl;
        // cout << gameboard << endl;
        mixhue_ret = flipPieces(gameboard, player + 1, temp.second);
        // cout << mixhue_ret << endl;
    }
    return temp.first;
}

string mixhue(int player, int d, int iterations, string gameboard)
{
    srand(time(0));
    mixhue_minimaxSearch(gameboard, player, player, d, 0, INT32_MIN, INT32_MAX, iterations);
    return mixhue_ret;
}
#endif