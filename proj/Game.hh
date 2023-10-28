#pragma once

#include "player.hh"

class Game {
    protected:
    Player* players = nullptr;
    int playersCount = 0;
    int inputs;
    int outputs;
    public:
    void SetPlayers(const int & _amount, Player* _players);
    virtual void Play();
    virtual int GetWinner();
    virtual float GetScore(const int & _playerId);
};