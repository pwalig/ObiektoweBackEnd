#pragma once

#include "Game.hh"

/*template <typename T>
class TicTacToeBoard {
    public:
    T board [3][3];

    template <typename T>
    void Fill(const T & value);
};*/

#define EMPTY_MARK ' '

// Tic Tac Toe Board
class TTTBoard {
    // board configuration:
    // 0 1 2
    // 3 4 5
    // 6 7 8
    char fields [9];

    public:
    TTTBoard();
    bool IsFull();
    void Reset();
    void Mark(const int & _id, const char & value);
    //char Mark(const int & _i, const int & _j, const char & value);
    char GetWinner();
    //void Render();

    friend class UTTTBoard;
    friend class UltimateTicTacToe;
};

// Ultimate Tic Tac Toe Board
class UTTTBoard {
    // board configuration:
    // 0 1 2
    // 3 4 5
    // 6 7 8
    TTTBoard boards [9];
    char wins [9];

    public:
    UTTTBoard();
    bool IsFull();
    void Reset();
    void Mark(const int & _board, const int & _field, const char & value);
    char GetWinner();
    TTTBoard GetBoardByIndex(const int & _id);
    //TTTBoard GetBoardByIndexes(const int & _i, const int & _j);
    
    void Render();
    void HelperWinners(const int & _curBoard);

    friend class UltimateTicTacToe;
};

class UltimateTicTacToe : public Game {
    UTTTBoard board;
    int curBoard;

    // available marks:
    // 0 - player 0 mark
    // 1 - player 1 mark
    char marks [2];
    
    bool display[2];

    public:
    UltimateTicTacToe();
    void Play();
};



// TESTS

void BoardTest();
void UltimateTicTacToeTest();