#ifndef historyheu // To make sure you don't declare the function more than once by including the header multiple times.
#define historyheu

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <string.h>
#include <time.h>
#include"MixFun.hpp"
using namespace std;
#define SIZE 6
string history_heu_ret;
int historyTable[6][6];

int minimaxSearch_hue(string gameboard, int originalplayer, int player, int depth, int d, int prune)
{
    // if (getValidMoves(gameboard, player).size() == 0)
    // {
    //     // cout << gameboard << endl;
    //     history_heu_ret = gameboard;
    //     return 0;
    // }
    if (prune == -100 && player != originalplayer - 1)
        prune = 100;
    else if (prune == 100 && player == originalplayer - 1)
        prune = -100;
    vector<pair<int, int>> moves = getValidMoves(gameboard, player);
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
    if (player != originalplayer - 1)
    {
        sort(moves.begin(), moves.end(), [&](const std::pair<int, int> &a, const std::pair<int, int> &b)
             { return historyTable[a.first][a.second] > historyTable[b.first][b.second]; });
    }
    else
    {
        sort(moves.begin(), moves.end(), [&](const std::pair<int, int> &a, const std::pair<int, int> &b)
             { return historyTable[a.first][a.second] < historyTable[b.first][b.second]; });
    }
    for (int i = 0; i < moves.size(); i++)
    {
        pos = 'A' + moves[i].first;
        pos += 'a' + moves[i].second;
        int val = minimaxSearch_hue(flipPieces(gameboard, player + 1, pos), originalplayer, (!player) + 1, depth, d + 1, prune);

        historyTable[moves[i].first][moves[i].second] += val;

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
    if (moves.size() == 0)
    {
        temp = {minimaxSearch_hue(gameboard, originalplayer, (!player) + 1, depth, d + 1, prune), ""};
    }
    if (d == 0)
    {
        // cout << flipPieces(gameboard, player, temp.second) << endl;
        history_heu_ret = flipPieces(gameboard, player+1, temp.second);
    }
    return temp.first;
}


string history_heu(int player, int depth, string gameboard)
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
        history_heu_ret = gameboard;
        return history_heu_ret;
    }

    minimaxSearch_hue(gameboard, player, player, depth, 0, -100);
    

    cout << history_heu_ret << endl;
    return history_heu_ret;
}
#endif