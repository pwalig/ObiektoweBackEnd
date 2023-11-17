#pragma once

#include <iostream>
#include <vector>

class Armour {
    public:
    virtual int CalculateNewDamage(const int & damage);

    // Required format:
    // <type = 'a'>
    virtual void Read(std::ifstream & in);
    virtual void Write(std::ofstream & out, const bool & f = true);
    virtual void PrintInfo(const bool & f = true);
    
    static Armour* GetNewArmour(std::ifstream & in);
};

class ConstArmour : public Armour {
    int reduction;
    
    public:
    int CalculateNewDamage(const int & damage) override;

    // Required format:
    // <type = 'c'> <reduction>
    void Read(std::ifstream & in) override;
    void Write(std::ofstream & out, const bool & f = true) override;
    void PrintInfo(const bool & f = true) override;
};

class ProportionalArmour : public Armour {
    double multiplier;

    public:
    int CalculateNewDamage(const int & damage) override;

    // Required format:
    // <type = 'p'> <multiplier>
    void Read(std::ifstream & in) override;
    void Write(std::ofstream & out, const bool & f = true) override;
    void PrintInfo(const bool & f = true) override;
};

// Class for combining other armours
// Its behaviour is defined purely by other armours it contains
// You can nest MultiArmours indefinetly
class MultiArmour : public Armour {
    std::vector<Armour*> subArmours;

    public:
    int CalculateNewDamage(const int & damage) override;

    // Required format:
    // <type = 'm'> <sub-armours amount> <sub-armour-1 type> <sub-armour-1 parameters> ...
    void Read(std::ifstream & in) override;
    void Write(std::ofstream & out, const bool & f = true) override;
    void PrintInfo(const bool & f = true) override;

    ~MultiArmour();
};