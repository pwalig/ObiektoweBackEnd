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

Game::~Game(){
    if (this->players != nullptr) delete [] this->players;
}

// get the amount of different actions you can take in the Game
int Game::GetInstanceInputs(){
    return 0;
}

// get the amount variables on which game state can be written
int Game::GetInstanceOutputs(){
    return 0;
}

int Game::GetInstanceRequiredPlayersCount(){
    return 0;
}

void Game::SetPlayers(const int & _amount, Player** _players){
    if (this->players != nullptr) delete [] players;
    this->playersCount = _amount;
    this->players = new Player* [this->playersCount];
    for (int i = 0; i < _amount; i++) {
        this->players[i] = _players[i];
    }
}

Game* Game::returnCopy(){
    Game* gg = new Game(*this);
    return gg;
}

void Game::Play(){ }

int Game::GetWinner(){
    return 0;
}

float Game::GetScore(const int & _playerId){
    return 0.0;
}