#ifndef MIXHUE
#define MIXHUE
#include <bits/stdc++.h>
#include "MixFun.hpp"
using namespace std;
#define SIZE 6
string mixhue_ret;
int mixhue_minimaxSearch(string gameboard, int originalplayer, int player, int depth, int d, int prune, int iteration);

int mixhue_minimaxSearch(string gameboard, int originalplayer, int player, int depth, int d, int prune, int iterations)
{
    // if (getValidMoves(gameboard, player).size() == 0)
    // {
    //     mixhue_ret = gameboard;
    //     return 0;
    // }
    if (prune == -100 && player != originalplayer - 1)
        prune = 100;
    else if (prune == 100 && player == originalplayer - 1)
        prune = -100;
    player--;
    if (d == depth)
    {
        // return countColorPieces(gameboard, originalplayer) - countColorPieces(gameboard, (!(originalplayer - 1)) + 1);
        int wins = 0;
        for (int i = 0; i < iterations; i++)
        {
            gameboard = randomGame(gameboard, originalplayer);
            double score = getScore(gameboard);
            if (score == 1.0 && player == 1 || score == 0.0 && player == 2)
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
                int val = mixhue_minimaxSearch(flipPieces(gameboard, player + 1, pos), originalplayer, (!player) + 1, depth, d + 1, prune, iterations);
                if (temp.second == "")
                {
                    temp.first = val;
                    temp.second = pos;
                    continue;
                }
                if (player == originalplayer - 1)
                {
                    if (val > prune)
                    {
                        return prune;
                    }
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
                }
                else
                {
                    if (val < prune)
                    {
                        return prune;
                    }

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
                }
            }
        }
    }
    if (!canmove)
    {
        temp = {mixhue_minimaxSearch(gameboard, originalplayer, (!player) + 1, depth, d + 1, prune, iterations), ""};
    }
    if (d == 0)
    {
        mixhue_ret = flipPieces(gameboard, player+1, temp.second);
    }
    return temp.first;
}

string mixhue(int player, int d, int iterations, string gameboard)
{
    srand(time(0));
    mixhue_minimaxSearch(gameboard, player, player, d, 0, -100, iterations);
    return mixhue_ret;
}
#endif