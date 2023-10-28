#include "Game.hh"

void Game::SetPlayers(const int & _amount, Player* _players){
    this->playersCount = _amount;
    this->players = _players;
}

void Game::Play(){ }

int Game::GetWinner(){
    return 0;
}

float Game::GetScore(const int & _playerId){
    return 0.0;
}