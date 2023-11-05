#pragma once
#include "player.hh"
#include "Being.hh"
#include <vector>
#include <string>

using namespace std;

class Game {
    protected:
    Player** players = nullptr;
    int playersCount = 0;
    public:
    Game();
    Game(const int & _amount, Player** _players);
    Game(const int & _amount, Player** _players, const int & _inputs, const int & _outputs);
    virtual ~Game();

    // get the amount of different actions you can take in the Game
    virtual int GetInstanceInputs();
    // get the amount of variables on which game state can be written
    virtual int GetInstanceOutputs();
    virtual int GetInstanceRequiredPlayersCount();
    virtual void SetPlayers(const int & _amount, Player** _players);

    virtual Game* returnCopy();

    virtual void Play();
    virtual int GetWinner();
    virtual float GetScore(const int & _playerId);
};

class Board{
    vector<vector<Being*>> fields;

    public:
    Board(int siz);

    void ReadBoardState(const string & filename);
    void SaveBoardState(const string & filename);

    friend class MainGame;
};

class MainGame{
    Board board;
    vector<Being*> playerBeings[2];

    public:
    void AddBeing(const int & playerId, Being* being, const int & x, const int & y);
    void ReadBoardState(const string & filename);
    void SaveBoardState(const string & filename);
    void Update();
    ~MainGame();
};

