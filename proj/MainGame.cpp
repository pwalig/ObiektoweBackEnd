#include "MainGame.hh"
#include <fstream>
#include <iostream>


//----------Board----------

Board::Board(const int & _siz)
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

Being* Board::GetBeing(const int & x, const int & y)
{
    return this->fields[x][y];
}

int Board::Move(const int & x1, const int & y1, const int & x2, const int & y2)
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

int Board::SetBeing(Being* b, const int & x, const int & y)
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


//----------MainGame----------

MainGame::MainGame(const string & filename):board(0){
    this->ReadBoardState(filename);
}

MainGame::~MainGame(){
    int playerCount = playerBeings.size();
    for(int i=0; i<playerCount; i++) {
        int siz_2 = this->playerBeings[i].size();
        for(int j=0; j<siz_2; j++)
        {
            delete this->playerBeings[i][j];
        }
        this->playerBeings[i].clear();
    }
}

void MainGame::ReadBoardState(const string & filename){
    ifstream fin(filename);
    int siz, playersCount;
    fin >> siz >> playersCount;
    this->board = siz;
    for (int i = 0; i < playersCount; i++)
    {
        int playerBCount;
        fin >> playerBCount;
        vector<Being*> tempBeings;
        for(int j = 0; j < playerBCount; j++){
            Being* nb = Being::GetNewBeing(fin);
            nb->SetGame(this);
            board.SetBeing(nb, nb->GetX(), nb->GetY());
            tempBeings.push_back(nb);
        }
        playerBeings.push_back(tempBeings);
    }
    fin.close();
}

void MainGame::SaveBoardState(const string & filename){

}

void MainGame::SortBeingsByPriotity(vector<Being*> & beings){
    
}

void MainGame::Update()
{
    int playersCount = playerBeings.size();
    for(int i=0; i < playersCount; i++) {
        int siz_2 = this->playerBeings[i].size();
        for(int j=0; j<siz_2; j++)
            this->playerBeings[i][j]->Act();
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

void MainGame::PrintInfo(){
    int psiz = playerBeings.size();
    cout << "board size: " << board.fields.size() << " players: " << psiz << endl;
    for (int i = 0; i < psiz; i++){
        int bsiz = playerBeings[i].size();
        cout << "player " << i << " beings: " << bsiz << endl;
        for (int j = 0; j < bsiz; j++){
            playerBeings[i][j]->PrintInfo();
        }
    }
}