#pragma once

#include <vector>
#include <string>

class Being;
#include "Being.hh"

using namespace std;

class Board{
    vector<vector<Being*>> fields;

    public:
    Board(const int & siz);

    void ReadBoardState(const string & filename);
    void SaveBoardState(const string & filename);
    int Move(const int & x1, const int & y1, const int & x2, const int & y2);
    Being* GetBeing(const int & x, const int & y);
    int SetBeing(Being* b, const int & x, const int & y);

    friend class MainGame;
};

class MainGame{
    Board board;
    vector<vector<Being*>> playerBeings; //playerBeings[player][being]

    void SortBeingsByPriotity(vector<Being*> & beings);

    public:
    MainGame(const string & filename);
    ~MainGame();

    void AddBeing(const int & playerId, Being* being, const int & x, const int & y);

    void ReadBoardState(const string & filename);
    void SaveBoardState(const string & filename);

    void Update();
    Board* GetBoard();
    void Destroy(Being* being);

    void PrintInfo();
};