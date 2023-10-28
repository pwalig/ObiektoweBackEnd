#pragma once

#include "Game.hh"

class Game2048: public Game{
    int board[4][4];
    int desiredValue;
    void Render();
    public:
    void Play();
    Game2048();
};