#ifndef abmc_H // To make sure you don't declare the function more than once by including the header multiple times.
#define abmc_H

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <string.h>
#include <time.h>
#include"MixFun.hpp"
using namespace std;
#define SIZE 6
int minimaxSearch(string gameboard, int originalplayer, int player, int depth, int d, int prune);
double posScore(int iterations, string gameboard, int player, string pos);
void monteCarlo(string gameboard, int player);
string mix_ret;


int minimaxSearch(string gameboard, int originalplayer, int player, int depth, int d, int prune)
{
    if (getValidMoves(gameboard, player).size() == 0)
    {
        // cout << gameboard << endl;
        mix_ret = gameboard;
        return 0;
    }
    if (prune == -100 && player != originalplayer - 1)
        prune = 100;
    else if (prune == 100 && player == originalplayer - 1)
        prune = -100;
    player--;
    if (d == depth)
    {
        return countColorPieces(gameboard, originalplayer) - countColorPieces(gameboard, (!(originalplayer - 1)) + 1);
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
                int val = minimaxSearch(flipPieces(gameboard, player + 1, pos), originalplayer, (!player) + 1, depth, d + 1, prune);
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
        temp = {minimaxSearch(gameboard, originalplayer, (!player) + 1, depth, d + 1, prune), ""};
    }
    if (d == 0)
    {
        // cout << flipPieces(gameboard, player, temp.second) << endl;
        mix_ret = flipPieces(gameboard, player, temp.second);
    }
    return temp.first;
}

double posScore(int iterations, string gameboard, int player, string pos)
{
    int wins = 0;
    gameboard = flipPieces(gameboard, player, pos);
    int tempplayer = (player == 1) ? 2 : 1;
    for (int i = 0; i < iterations; i++)
    {
        gameboard = randomGame(gameboard, tempplayer);
        double score = getScore(gameboard);
        if (score == 1.0 && player == 1 || score == 0.0 && player == 2)
        {
            wins++;
        }
    }
    return (double)wins / (double)iterations;
}

void monteCarlo(string gameboard, int player, int iteration)
{
    if (getValidMoves(gameboard, player).size() == 0)
    {
        // cout << gameboard << endl;
        mix_ret = gameboard;
        return;
    }
    vector<pair<int, int>> move = getValidMoves(gameboard, player);
    double best = -1;
    vector<string> bestmoves;
    for (int i = 0; i < move.size(); i++)
    {
        string pos = "";
        pos += 'A' + move[i].first;
        pos += 'a' + move[i].second;
        double temp = posScore(iteration, gameboard, player, pos);
        if (temp > best)
        {
            bestmoves.clear();
            bestmoves.push_back(pos);
            best = temp;
        }
    }
    int moveIndex = rand() % bestmoves.size();
    // cout << flipPieces(gameboard, player, bestmoves[moveIndex]) << endl;
    mix_ret = flipPieces(gameboard, player, bestmoves[moveIndex]);
}

string mix_depth(int player, int depth, int iteration, string gameboard)
{
    srand(time(0));
    // int player, depth;
    // cin >> player;
    // cin >> depth;
    // string gameboard;
    // cin >> gameboard;

    if (gameOver(gameboard))
    {
        // cout<< gameboard;
        mix_ret = gameboard;
        return mix_ret;
    }
    if (countRound(gameboard) < SIZE * SIZE / 2)
    {
        minimaxSearch(gameboard, player, player, depth, 0, -100);
    }
    else
    {
        monteCarlo(gameboard, player, iteration);
    }
    return mix_ret;
}
#endif