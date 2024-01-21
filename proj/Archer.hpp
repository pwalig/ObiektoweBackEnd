#pragma once

#include "MoveAble.hpp"

#define ARCHER_CHAR 'a'

class Archer : public MoveAble{
    protected:
    int damage = 10;
    int range = 3;

    public:
    virtual void Act();

    // Required format:
    // <type = 'h'> <x> <y> <priority> <hp> <armour type> <armour parameters>
    virtual void Read(std::ifstream & in);
    virtual void Read(const nlohmann::json & data);
    virtual void Write(std::ofstream & out, const bool & f = true) const;
    virtual nlohmann::json Write();
    virtual void PrintInfo(const bool & f = true);
};