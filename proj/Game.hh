#pragma once

#include "player.hh"

class Game {
    protected:
    Player** players = nullptr;
    int playersCount = 0;
    int inputs; // the amount of different actions you can take in the Game
    int outputs; // the amount variables on which game state can be written
    public:
    Game();
    Game(const int & _amount, Player** _players);
    Game(const int & _amount, Player** _players, const int & _inputs, const int & _outputs);

    void SetInstanceInputs(const int & _inputs);
    void SetInstanceOutputs(const int & _outputs);
    int GetInstanceInputs();
    int GetInstanceOutputs();
    virtual void SetPlayers(const int & _amount, Player** _players);

    virtual void Play();
    virtual int GetWinner();
    virtual float GetScore(const int & _playerId);
};