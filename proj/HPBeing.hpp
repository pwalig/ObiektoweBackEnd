#pragma once

#include "Armour.hh"
#include "Being.hh"

#define HPBEING_CHAR 'h'

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
    virtual void Read(const nlohmann::json & data);
    virtual void Write(std::ofstream & out, const bool & f = true) const;
    virtual nlohmann::json Write();
    virtual void PrintInfo(const bool & f = true);

    ~HPBeing();
};