#include "MainGame.hh"
#include <fstream>
#include <string>
#include <iostream>
#include <algorithm>
#include "json.hpp"

using nlohmann::json;
using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::string;
using std::vector;

//----------Board----------

Board::Board(const int &_siz)
{
    for (int i = 0; i < _siz; i++)
    {
        vector<Being *> vct;
        for (int j = 0; j < _siz; j++)
        {
            vct.push_back(nullptr);
        }
        this->fields.push_back(vct);
    }
}

int Board::GetFieldsSize() const
{
    return this->fields.size();
}
Being *Board::GetBeing(const int &x, const int &y)
{
    if (x < 0 || x >= this->fields.size() || y < 0 || y >= this->fields[x].size()) return nullptr;
    return this->fields[x][y];
}

int Board::Move(const int &x1, const int &y1, const int &x2, const int &y2)
{
    if (x1 < 0 || x1 >= this->fields.size() || y1 < 0 || y1 >= this->fields[x1].size() || x2 < 0 || x2 >= this->fields.size() || y2 < 0 || y2 >= this->fields[x2].size())
        return 1; // error code - coords are out of bounds
    if (this->fields[x1][y1] == nullptr || this->fields[x2][y2] != nullptr)
        return 2; // error code - existence requirements unmet
    this->fields[x2][y2] = this->fields[x1][y1];
    this->fields[x1][y1] = nullptr;
    return 0;
}

int Board::SetBeing(Being *b, const int &x, const int &y)
{
    if (x < 0 || x >= this->fields.size() || y < 0 || y >= this->fields[x].size())
        return 1; // error code - being to be set out of bounds
    this->fields[x][y] = b;
    return 0;
}

int Board::AddBeing(Being *b, const int &x, const int &y)
{
    if (this->fields[x][y] != nullptr)
        return 1; // error code - overriding an existing being
    this->SetBeing(b, x, y);
    return 0;
}

int Board::RemoveBeing(Being *b, const int &x, const int &y)
{
    if (this->fields[x][y] == nullptr)
        return 1; // error code - removing from an empty field
    this->fields[x][y] = nullptr;
    return 0;
}

//----------MainGame----------
MainGame::MainGame(const string &filename, const int &format) : board(0){
    this->ReadBoardState(filename, format);
}

MainGame::~MainGame()
{
    int playerCount = playerBeings.size();
    for (int i = 0; i < playerCount; i++)
    {
        int siz_2 = this->playerBeings[i].size();
        for (int j = 0; j < siz_2; j++)
        {
            delete this->playerBeings[i][j];
        }
        this->playerBeings[i].clear();
    }
}

void MainGame::ReadBoardState(const string &filename, const int &format)
{
    ifstream fin(filename);
    switch (format)
    {
    case jsonReading:
    {
        json j_board = json::parse(fin);
        int siz = j_board["size"];
        this->board = siz;
        int playersCount = j_board["players"];
        for (int i = 0; i < playersCount; i++)
        {
            vector<Being *> empty_vector;
            playerBeings.push_back(empty_vector);
        }
        int playerBCount = j_board["beings"].size();
            for (int i = 0; i < playerBCount; i++)
            {
                Being *nb = Being::GetNewBeing(j_board["beings"][i]);
                nb->SetGame(this);
                board.SetBeing(nb, nb->GetX(), nb->GetY());
                playerBeings[nb->GetOwner()].push_back(nb);
            }
        break;
    }
    default:
        int siz, playersCount;
        fin >> siz >> playersCount;
        this->board = siz;
        for (int i = 0; i < playersCount; i++)
        {
            int playerBCount;
            fin >> playerBCount;
            vector<Being *> tempBeings;
            for (int j = 0; j < playerBCount; j++)
            {
                Being *nb = Being::GetNewBeing(fin);
                nb->SetGame(this);
                board.SetBeing(nb, nb->GetX(), nb->GetY());
                tempBeings.push_back(nb);
            }
            playerBeings.push_back(tempBeings);
        }
        break;
    }
    fin.close();
}

void MainGame::SaveBoardState(const string &filename, const int &format)
{
    ofstream fout(filename);
    switch (format)
    {
    case jsonReading:
    {
        json jBoard;
        jBoard["size"] = this->board.fields.size();
        jBoard["players"] = this->playerBeings.size();
        int counter=0;
        for (int i = 0; i < this->board.fields.size(); i++)
            for (int j = 0; j < this->board.fields[i].size(); j++)
                if (this->board.fields[i][j] != nullptr)
                {
                    json data = this->board.GetBeing(i, j)->Write();
                    jBoard["beings"][counter]=data;
                    counter++;
                }
        fout << jBoard.dump(4);
        break;
    }
    default:
    {
        /*int siz, playersCount;
        fout << siz << playersCount;
        this->board = siz;
        for (int i = 0; i < playersCount; i++)
        {
            int playerBCount;
            fout << playerBCount;
            vector<Being *> tempBeings;
            for (int j = 0; j < playerBCount; j++)
            {
                Being *nb = Being::Write(fout);
                nb->SetGame(this);
                board.SetBeing(nb, nb->GetX(), nb->GetY());
                tempBeings.push_back(nb);
            }
            playerBeings.push_back(tempBeings);
        }*/
        break;
    }
    }
    fout.close();
}

void MainGame::SortBeingsByPriority()
{
    for (int i = 0; i < playerBeings.size(); i++) {
        std::sort(playerBeings[i].begin(), playerBeings[i].end(), [](const Being* a, const Being* b)
        { 
            return a->GetPriority() < b->GetPriority();
        });
    }
}

vector<Being *> MainGame::GetCallQueue()
{
    vector<Being *> callQueue;
    int playersCount = playerBeings.size();
    for (int i = 0; i < playersCount; i++)
    {
        int pbsiz = playerBeings[i].size();
        for (int j = 0; j < pbsiz; j++)
        {
            callQueue.push_back(playerBeings[i][j]);
        }
    }
    std::sort(callQueue.begin(), callQueue.end(), [](const Being* a, const Being* b)
    { 
        return a->GetPriority() < b->GetPriority();
    });
    return callQueue;
}

void MainGame::Update()
{
    vector<Being *> callQueue = GetCallQueue();
    int siz = callQueue.size();
    for (int i = 0; i < siz; i++)
        callQueue[i]->Act();
}

void MainGame::UpdateByPlayer()
{
    SortBeingsByPriority();
    int playersCount = playerBeings.size();
    for (int i = 0; i < playersCount; i++)
    {
        int siz_2 = this->playerBeings[i].size();
        for (int j = 0; j < siz_2; j++)
            this->playerBeings[i][j]->Act();
    }
}

Board *MainGame::GetBoard()
{
    return &this->board;
}

void MainGame::Destroy(Being *being)
{
    int x = being->GetX();
    int y = being->GetY();
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < playerBeings[i].size(); j++)
        {
            if (playerBeings[i][j]->GetX() == x && playerBeings[i][j]->GetY() == y)
            {
                playerBeings[i].erase(playerBeings[i].begin() + j);
                break;
            }
        }
    delete being;
    this->board.fields[x][y] = nullptr;
}

void MainGame::Play(const string &filename, const int &format, int iterations)
{
    /*
    bool end=true;
    for(int i=0; i<playerBeings.size(); i++)
        if(playerBeings[i].size())
            end=false;
    if(end)
        break;
    */
    for(int ctr=0; ctr<iterations; ctr++)
    {
        this->Update();
        cout << std::to_string(ctr)+filename << endl;
        this->SaveBoardState(filename + std::to_string(ctr)+ ".json", format);
    }
    return;
}

void MainGame::PrintInfo()
{
    int psiz = playerBeings.size();
    cout << "board size: " << board.fields.size() << " players: " << psiz << endl;
    for (int i = 0; i < psiz; i++)
    {
        int bsiz = playerBeings[i].size();
        cout << "player " << i << " beings: " << bsiz << endl;
        for (int j = 0; j < bsiz; j++)
        {
            playerBeings[i][j]->PrintInfo();
        }
    }
}