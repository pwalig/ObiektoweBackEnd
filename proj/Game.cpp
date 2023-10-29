#include "Game.hh"


Game::Game():playersCount(0), players(nullptr){ }

Game::Game(const int & _amount, Player** _players):playersCount(_amount), players(nullptr) {
    players = new Player* [_amount];
    for (int i = 0; i < _amount; i++) {
        this->players[i] = _players[i];
    }
}

Game::Game(const int & _amount, Player** _players, const int & _inputs, const int & _outputs):playersCount(_amount), players(nullptr) {
    players = new Player* [_amount];
    for (int i = 0; i < _amount; i++) {
        this->players[i] = _players[i];
    }
}

// get the amount of different actions you can take in the Game
int Game::GetInstanceInputs(){
    return 0;
}

// get the amount variables on which game state can be written
int Game::GetInstanceOutputs(){
    return 0;
}

void Game::SetPlayers(const int & _amount, Player** _players){
    if (players) delete [] players;
    this->playersCount = _amount;
    players = new Player* [_amount];
    for (int i = 0; i < _amount; i++) {
        this->players[i] = _players[i];
    }
}

void Game::Play(){ }

int Game::GetWinner(){
    return 0;
}

float Game::GetScore(const int & _playerId){
    return 0.0;
}