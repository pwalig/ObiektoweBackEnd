#include "Game.hh"
#include <fstream>


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

/*void Board::UpdateBoard()
{
    int siz = this->fields.size();
    for(int i=0; i<siz; i++)
    {
        int siz_2 = this->fields[i].size();
        for(int j=0; j<siz_2; j++)
            this->fields[i][j]->Act();
    }
}*/


void MainGame::Update()
{
    for(int i=0; i<2; i++) {
        int siz_2 = this->playerBeings[i].size();
        for(int j=0; j<siz_2; j++)
            this->playerBeings[i][j]->Act();
    }
}

void MainGame::ReadBoardState(const string & filename){
    ifstream fin(filename);
    int siz, player1NumOfBeings, player2NumOfBeings;
    fin >> siz;
    //the rest
    for(int i=0; i<siz; i++)
    {
        vector<Being*> vct;
        for(int j=0; j<siz; j++)
        {
            
            vct.push_back(nullptr); 
        }
        this->board.fields.push_back(vct);
    }
    fin.close();
}

void MainGame::SaveBoardState(const string & filename){

}

MainGame::~MainGame(){
    for(int i=0; i<2; i++) {
        int siz_2 = this->playerBeings[i].size();
        for(int j=0; j<siz_2; j++)
        {
            delete this->playerBeings[i][j];
        }
        this->playerBeings[i].clear();
    }
}