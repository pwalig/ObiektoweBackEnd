#pragma once

#include <iostream>
#include <string>
#include <vector>

class MainGame;
#include "MainGame.hh"
#include "json.hpp"
#include "Armour.hh"
#include "Being.hh"


#define KNIGHT_CHAR 'h'

class Knight : HPBeing{
    protected:
    int baseDamage;
    public:
    virtual void Act();
    
    int GetBaseDamage() const;
    virtual void SetBaseDamage(const int dam);

    virtual void Read(std::ifstream & in);
    virtual void Read(const nlohmann::json & data);
    virtual void Write(std::ofstream & out, const bool & f = true) const;
    virtual nlohmann::json Write();
    virtual void PrintInfo(const bool & f = true);
};
