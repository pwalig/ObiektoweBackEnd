#pragma once

#include <iostream>
#include <string>
#include <vector>

class MainGame;
#include "MainGame.hh"
#include "json.hpp"

class Being{
    protected:
    int x;
    int y;
    int owner;
    int priority;
    MainGame* game;

    public:
    int GetX() const; 
    int GetY() const;
    void SetX(const int val);
    void SetY(const int val);
    void SetPriority(const int val);
    void SetOwner(const int val);
    int GetOwner() const;
    int GetPriority() const;
    void SetGame(MainGame* mg);
    

    virtual void Act();

    // Required format:
    // <type = 'b'> <x> <y> <priority>
    virtual void Read(std::ifstream & in);
    virtual void Read(const nlohmann::json & data);
    virtual void Write(std::ofstream & out, const bool & f = true) const;
    virtual nlohmann::json Write();
    virtual void PrintInfo(const bool & f = true);
    
    static Being* GetNewBeing(const char & in);
    static Being* GetNewBeing(std::ifstream & in);
    static Being* GetNewBeing(const nlohmann::json & data);
};

std::ifstream& operator >> (std::ifstream & is, Being & being);

std::ofstream& operator << (std::ofstream & os, const Being & being);