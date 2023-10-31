#pragma once

#include "player.hh"

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

    virtual void Play();
    virtual int GetWinner();
    virtual float GetScore(const int & _playerId);
};