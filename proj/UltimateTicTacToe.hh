#pragma once

#include "Game.hh"

template <typename T>
class TicTacToeBoard{
    T[3][3] board;
};

class UltimateTicTacToe : public Game {
    TicTacToeBoard<TicTacToeBoard<char>> board;
    public:
    UltimateTicTacToe();
    void Play();
};