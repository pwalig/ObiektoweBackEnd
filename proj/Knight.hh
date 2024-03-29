#pragma once

#include "HPBeing.hpp"


#define KNIGHT_CHAR 'k'

class Knight : public HPBeing{
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
