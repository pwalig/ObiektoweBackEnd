#pragma once

#include "player.hh"

class Game {
    private:
    Player* players = nullptr;
    int inputs;
    int outputs;
    public:
    void SetPlayers(const int & _amount, Player* _players);
    virtual void Play();
};