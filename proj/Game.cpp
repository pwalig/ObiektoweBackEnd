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

Board::Board(int _siz)
{
    for(int i=0; i<_siz; i++)
    {
        vector<Being*> vct;
        for(int j=0; j<_siz; j++)
        {
            
            vct.push_back(nullptr); 
        }
        this->fields.push_back(vct);
    }
}

Being* Board::GetBeing(int x, int y)
{
    return this->fields[x][y];
}

int Board::Move(int x1, int x2, int y1, int y2)
{
    if(x1 < 0 || x1 >= this->fields.size())
        return 1;
    if(y1 < 0 || y1 >= this->fields[x1].size())
        return 2;
    if(x2 < 0 || x2 >= this->fields.size())
        return 3;
    if(y2 < 0 || y2 >= this->fields[x2].size())
        return 4;
    if(this->fields[x1][y1] == nullptr)
        return 5;
    if(this->fields[x2][y2] != nullptr)
        return 6;
    this->fields[x2][y2] = this->fields[x1][y1];
    this->fields[x1][y1] = nullptr;
    return 0;
}

int Board::SetBeing(Being* b, int x, int y)
{
    if(x < 0 || x >= this->fields.size())
        return 1;
    if(y < 0 || y >= this->fields[x].size())
        return 2;
    if(b == nullptr)
        return 3;
    if(this->fields[x][y] != nullptr)
        return 4;
    this->fields[x][y] = b;
    return 0;
}

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
    int siz, playerOneBeingCount, playerTwoBeingCount;
    fin >> siz;
    this->board = siz;
    fin >> playerOneBeingCount >> playerTwoBeingCount;
    for(int i=0; i<playerOneBeingCount; i++)
        playerBeings[0].push_back(Being::GetNewBeing(fin));
    for(int i=0; i<playerOneBeingCount; i++)
        playerBeings[1].push_back(Being::GetNewBeing(fin));
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

Board* MainGame::GetBoard()
{
    return &this->board;
}

void MainGame::Destroy(Being* being)
{
    int x = being->GetX();
    int y = being->GetY();
    this->board.fields[x][y] = nullptr;
}