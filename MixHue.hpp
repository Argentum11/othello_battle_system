#ifndef MIXHUE
#define MIXHUE
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
int minimaxSearch(string gameboard, int originalplayer, int player, int depth, int d, int prune,int iterations);
double getScore(string gameboard);
bool isValidMove(string gameboard, string pos, int player);
vector<pair<int, int>> getValidMoves(string gameboard, int player);
bool gameOver(string gameboard);
string randomGame(string gameboard, int player);
int posScore(int iterations, string gameboard, int player, string pos);
void monteCarlo(string gameboard, int player);
string mixhue_ret;
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

int minimaxSearch(string gameboard, int originalplayer, int player, int depth, int d, int prune, int iterations)
{
    if (getValidMoves(gameboard, player).size() == 0)
    {
        cout << gameboard << endl;
        return 0;
    }
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
        cout << wins << endl;
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
                int val = minimaxSearch(flipPieces(gameboard, player + 1, pos), originalplayer, (!player) + 1, depth, d + 1, prune,iterations);
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
        temp = {minimaxSearch(gameboard, originalplayer, (!player) + 1, depth, d + 1, prune,iterations), ""};
    }
    if (d == 0)
    {
        mixhue_ret = flipPieces(gameboard, player, temp.second);
    }
    return temp.first;
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

string mixhue(int player,int d,int iterations,string gameboard)
{
    srand(time(0));
    minimaxSearch(gameboard,player,player,d,0,-100,iterations);
    return mixhue_ret;
}
#endif