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
    bool Draw();
    void Reset();
    void Mark(const int & _board, const int & _field, const char & value);
    char GetWinner();
    TTTBoard GetBoardByIndex(const int & _id);
    //TTTBoard GetBoardByIndexes(const int & _i, const int & _j);
    
    void Render();
    void HelperBoard(const int & _curBoard);

    friend class UltimateTicTacToe;
};

class UltimateTicTacToe : public Game {
    UTTTBoard board;
    int curBoard;
    
    const static int requiredPlayersCount;
    const static int inputs;
    const static int outputs;
    bool display[2];
    float scores[2];

    float wrongMovePenalty;
    float giveBoardChoicePenalty;
    float wonBoardReward;
    float lostBoardPenalty;
    float wonGameReward;
    float lostGamePenalty;
    
    bool forceDisplay;

    // available marks:
    // 0 - player 0 mark
    // 1 - player 1 mark
    char marks [2];
    
    float* GetBoardState(const int & _playerId);
    float* GetCompressedBoardState(const int & _playerId);

    public:
    //Creating TicTacToe game with no players can cause errors.
    //Remember to add players before you call Play();
    UltimateTicTacToe();
    UltimateTicTacToe(const int & _amount, Player** _players);
    
    virtual void SetPlayers(const int & _amount, Player** _players);
    void SetForeDisplay(const bool & _disp);
    void SetScoreParameters(const float & _wrongMovePenalty, const float & _giveBoardChoicePenalty, const float & _wonBoardReward, const float & _lostBoardPenalty, const float & _wonGameReward, const float & _lostGamePenalty);

    // get the amount of different actions you can take in the Game
    static int GetInputs();
    // get the amount of variables on which game state can be written
    static int GetOutputs();
    static int GetRequiredPlayersCount();
    // get the amount of different actions you can take in the Game
    virtual int GetInstanceInputs();
    // get the amount of variables on which game state can be written
    virtual int GetInstanceOutputs();
    virtual int GetInstanceRequiredPlayersCount();

    virtual Game* returnCopy();

    virtual void Play();
    virtual int GetWinner();
    virtual float GetScore(const int & _playerId);
};