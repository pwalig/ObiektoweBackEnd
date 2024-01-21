#pragma once

#include "HPBeing.hpp"

#define MOVEABLE_CHAR 'm'

class MoveAble : public HPBeing{
    protected:
    int moveX = 0;
    int moveY = 0;

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