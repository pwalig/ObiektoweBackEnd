#pragma once


#include <fstream>
#include <string>
using namespace std;

class MainGame;

class Being{
    protected:
    int x;
    int y;
    int priority;
    MainGame* game;

    public:
    int GetX();
    int GetY();
    int GetPriority();
    void SetGame(MainGame* mg);

    virtual void Act();
    virtual void Read(ifstream & in);
    virtual void Write(ofstream & out, const bool & f = true);
    virtual void PrintInfo(const bool & f = true);
    
    static Being* GetNewBeing(ifstream & in);
    static Being* GetNewBeing(string filename);
};

class Living : public Being{
    protected:
    int hp = 0;

    public:
    virtual void Act();
    virtual void Read(ifstream & in);
    virtual void Write(ofstream & out, const bool & f = true);
    virtual void PrintInfo(const bool & f = true);
};

class TestBeing : public Being{
    int value = 0;

    public:
    virtual void Act();
    virtual void Read(ifstream & in);
    virtual void Write(ofstream & out, const bool & f = true);
    virtual void PrintInfo(const bool & f = true);
};