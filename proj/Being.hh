#pragma once

#include <iostream>
#include <string>
#include <vector>

class MainGame;
#include "MainGame.hh"
#include "Armour.hh"

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

    // Required format:
    // <type = 'b'> <x> <y> <priority>
    virtual void Read(std::ifstream & in);
    virtual void Write(std::ofstream & out, const bool & f = true);
    virtual void PrintInfo(const bool & f = true);
    
    static Being* GetNewBeing(std::ifstream & in);
    static Being* GetNewBeing(std::string filename);
    static int partitionBeingsByPriority(std::vector<Being*> & beings, const int & start, const int & end, const bool & ascendingOrder);
    static void quickSortBeingsByPriority(std::vector<Being*> & beings, const int & start, const int & end, const bool & ascendingOrder);
};

class HPBeing : public Being{
    protected:
    int hp = 0;
    Armour* armour;

    public:
    virtual void Act();

    // Reduces this Being's hp.
    // Destroys it if hp goes below 0.
    virtual void DealDamage(const int & damage);

    // Reduces Being's hp.
    // Destroys it if hp goes below 0.
    // Returns remaining hp.
    // Warning! returned value can be below 0, when damage exceeds remaining hp.
    static int DealDamage(HPBeing* hpb, const int & damage);

    // Required format:
    // <type = 'h'> <x> <y> <priority> <hp> <armour type> <armour parameters>
    virtual void Read(std::ifstream & in);
    virtual void Write(std::ofstream & out, const bool & f = true);
    virtual void PrintInfo(const bool & f = true);

    ~HPBeing();
};

class TestBeing : public Being{
    int value = 0;

    public:
    virtual void Act();

    // Required format:
    // <type = 't'> <x> <y> <priority> <value>
    virtual void Read(std::ifstream & in);
    virtual void Write(std::ofstream & out, const bool & f = true);
    virtual void PrintInfo(const bool & f = true);
};