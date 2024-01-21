#pragma once

#include <iostream>
#include <string>
#include <vector>

class MainGame;
#include "MainGame.hh"
#include "json.hpp"
#include "Armour.hh"

class Being{
    protected:
    int x;
    int y;
    int owner;
    int priority;
    MainGame* game;

    public:
    int GetX();
    int GetY();
    void SetX(const int val);
    void SetY(const int val);
    void SetPriority(const int val);
    void SetOwner(const int val);
    int GetOwner();
    int GetPriority();
    void SetGame(MainGame* mg);
    

    virtual void Act();

    // Required format:
    // <type = 'b'> <x> <y> <priority>
    virtual void Read(std::ifstream & in);
    virtual void Read(nlohmann::json & data);
    virtual void Write(std::ofstream & out, const bool & f = true) const;
    virtual nlohmann::json Write();
    virtual void PrintInfo(const bool & f = true);
    
    static Being* GetNewBeing(std::ifstream & in);
    static Being* GetNewBeing(nlohmann::json data);
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
    virtual void Read(nlohmann::json & data);
    virtual void Write(std::ofstream & out, const bool & f = true) const;
    virtual nlohmann::json Write();
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
    virtual void Read(nlohmann::json & data);
    virtual void Write(std::ofstream & out, const bool & f = true) const;
    virtual nlohmann::json Write();
    virtual void PrintInfo(const bool & f = true);
};

std::ifstream& operator >> (std::ifstream & is, Being & being);

std::ofstream& operator << (std::ofstream & os, const Being & being);