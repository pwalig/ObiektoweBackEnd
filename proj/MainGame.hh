#pragma once

#include <vector>
#include <string>

class Being;
#include "Being.hh"

class Board{
    std::vector<std::vector<Being*>> fields;

    public:
    Board(const int & siz);

    void ReadBoardState(const std::string & filename);
    void SaveBoardState(const std::string & filename);
    int Move(const int & x1, const int & y1, const int & x2, const int & y2);
    Being* GetBeing(const int & x, const int & y);
    int SetBeing(Being* b, const int & x, const int & y);
    int AddBeing(Being* b, const int & x, const int & y);
    int RemoveBeing(Being* b, const int & x, const int & y);

    friend class MainGame;
};

// Does not have default constructor!
// To construct object of this class use:
// MainGame <object name> = "filename";
// or
// MainGame <object name>("filename");
class MainGame{
    Board board;
    std::vector<std::vector<Being*>> playerBeings; // organisation: playerBeings[player][being]

    void SortBeingsByPriority();
    std::vector<Being*> GetCallQueue();

    public:
    MainGame(const std::string & filename);
    ~MainGame();

    void AddBeing(const int & playerId, Being* being, const int & x, const int & y);

    void ReadBoardState(const std::string & filename);
    void SaveBoardState(const std::string & filename);

    void Update();
    void UpdateByPlayer();
    Board* GetBoard();
    void Destroy(Being* being);

    void Play(const std::string & filename);
    void PrintInfo();
};