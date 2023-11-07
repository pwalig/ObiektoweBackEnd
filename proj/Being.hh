#pragma once

#include <fstream>
#include <string>

class MainGame;
#include "Game.hh"

using namespace std;

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

class HPBeing : public Being{
    protected:
    int hp = 0;

    public:
    virtual void Act();

    // Reduces Being's hp.
    // Destroys it if hp goes below 0.
    virtual void DealDamage(const int & damage);

    // Reduces Being's hp.
    // Destroys it if hp goes below 0.
    // Returns remaining hp.
    // Warning! returned value can be below 0, when damage exceeds remaining hp.
    static int DealDamage(HPBeing* hpb, const int & damage);

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