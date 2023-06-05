#ifndef MixFun
#define MixFun
#include <bits/stdc++.h>
using namespace std;
#define SIZE 6
pair<int, int> direction[] = {
    {-1, 0},
    {-1, 1},
    {0, 1},
    {1, 1},
    {1, 0},
    {1, -1},
    {0, -1},
    {-1, -1}};

int countRound(string s);
int countFlipPieces(string s, int player, string pos, int dir);
string flipPieces(string s, int player, string pos);
int countColorPieces(string s, int player);
double getScore(string gameboard);
bool isValidMove(string gameboard, string pos, int player);
vector<pair<int, int>> getValidMoves(string gameboard, int player);
bool gameOver(string gameboard);
string randomGame(string gameboard, int player);
int countRound(string s)
{
    int ret = 0;
    for (int i = 0; i < s.size(); i++)
    {
        if (s[i] != '-')
            ret++;
    }
    return ret;
}

int countFlipPieces(string s, int player, string pos, int dir)
{
    char board[SIZE][SIZE];
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            board[i][j] = s[i * SIZE + j];
        }
    }
    int posx = pos[0] - 'A';
    int posy = pos[1] - 'a';
    if (board[posx][posy] != '-')
        return 0;
    posx += direction[dir].first;
    posy += direction[dir].second;
    int ans = 0;
    while (0 <= posx && posx <= 5 && 0 <= posy && posy <= 5)
    {
        if (player == 1)
        {
            if (board[posx][posy] == 'W')
                ans++;
            else if (board[posx][posy] == 'B')
                return ans;
            else
                return 0;
        }
        else if (player == 2)
        {
            if (board[posx][posy] == 'B')
                ans++;
            else if (board[posx][posy] == 'W')
                return ans;
            else
                return 0;
        }
        posx += direction[dir].first;
        posy += direction[dir].second;
    }
    return 0;
}

string flipPieces(string s, int player, string pos)
{
    // cout << "gameboard1 "<< endl;
    // for (int i = 0; i < 6; i++)
    // {
    //     for (int j = 0; j < 6; j++)
    //     {
    //         cout << s[i*6+j];
    //     }
    //     cout << endl;
    // }
    // cout << player << endl;
    if(pos=="")
        return s;
    char board[SIZE][SIZE];
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            board[i][j] = s[i * SIZE + j];
        }
    }
    int posx = pos[0] - 'A';
    int posy = pos[1] - 'a';
    board[posx][posy] = (player == 1 ? 'B' : 'W');
    for (int i = 0; i < 8; i++)
    {
        int temp = countFlipPieces(s, player, pos, i);
        if (temp > 0)
        {
            for (int j = 1; j <= temp; j++)
            {
                board[posx + direction[i].first * j][posy + direction[i].second * j] = (player == 1 ? 'B' : 'W');
            }
        }
    }
    string ret = "";
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            ret += board[i][j];
        }
    }
    // cout << "gameboard2 "<<endl;
    // for (int i = 0; i < 6; i++)
    // {
    //     for (int j = 0; j < 6; j++)
    //     {
    //         cout << ret[i*6+j];
    //     }
    //     cout << endl;
    // }
    // cout << "p\n";
    // cout << player<<endl;
    return ret;
}

int countColorPieces(string s, int player)
{
    if (player == 1)
    {
        int ans = 0;
        for (char i : s)
        {
            if (i == 'B')
                ans++;
        }
        return ans;
    }
    else if (player == 2)
    {
        int ans = 0;
        for (char i : s)
        {
            if (i == 'W')
                ans++;
        }
        return ans;
    }
    return 0;
}

double getScore(string gameboard)
{
    int blackCount = countColorPieces(gameboard, 1);
    int whiteCount = countColorPieces(gameboard, 2);

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

bool isValidMove(string gameboard, string pos, int player)
{
    for (int i = 0; i < 8; i++)
    {
        if (countFlipPieces(gameboard, player, pos, i))
            return true;
    }
    return false;
}

vector<pair<int, int>> getValidMoves(string gameboard, int player)
{
    vector<pair<int, int>> validMoves;
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            string pos = "";
            pos += 'A' + i;
            pos += 'a' + j;
            if (isValidMove(gameboard, pos, player))
            {
                validMoves.push_back(make_pair(i, j));
            }
        }
    }
    return validMoves;
}

bool gameOver(string gameboard)
{
    if (getValidMoves(gameboard, 1).size() || getValidMoves(gameboard, 2).size())
        return false;
    return true;
}

string randomGame(string gameboard, int player)
{
    while (true)
    {
        if (gameOver(gameboard))
        {
            break;
        }
        vector<pair<int, int>> validMoves = getValidMoves(gameboard, player);
        if (validMoves.size() > 0)
        {
            int moveIndex = rand() % validMoves.size();
            string pos = "";
            pos += 'A' + validMoves[moveIndex].first;
            pos += 'a' + validMoves[moveIndex].second;
            gameboard = flipPieces(gameboard, player, pos);
            player = (player == 1) ? 2 : 1;
        }
        else
        {
            player = (player == 1) ? 2 : 1;
        }
    }
    return gameboard;
}
#endif