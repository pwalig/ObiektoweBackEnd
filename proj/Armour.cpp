#include "Armour.hh"
#include <fstream>

using std::ifstream;
using std::ofstream;
using std::string;
using std::cout;
using std::endl;

#define ARMOUR_CHAR 'a'
#define CONST_ARMOUR_CHAR 'c'
#define PROPORTIONAL_ARMOUR_CHAR 'p'
#define MULTI_ARMOUR_CHAR 'm'


//----------Armour Static Methods----------

Armour* Armour::GetNewArmour(string filename){
    ifstream in;
    in.open(filename);
    Armour* b = Armour::GetNewArmour(in);
    in.close();
    return b;
}

Armour* Armour::GetNewArmour(ifstream & in){
    char t = 'c';
    in >> t;
    Armour* a;
    switch (t)
    {        
    case ARMOUR_CHAR:
        a = new Armour();
        break;

    case CONST_ARMOUR_CHAR:
        a = new ConstArmour();
        break;
        
    case PROPORTIONAL_ARMOUR_CHAR:
        a = new ProportionalArmour();
        break;

    case MULTI_ARMOUR_CHAR:
        a = new MultiArmour();
        break;

    default:
        a = new Armour();
        break;
    }
    a->Read(in);
    return a;
}


//----------Armour----------

int Armour::CalculateNewDamage(const int & damage) {
    return damage;
}

void Armour::Read(ifstream & in) {
}

void Armour::Write(std::ofstream & out, const bool & f) const {
    out << ARMOUR_CHAR << " ";
}

void Armour::PrintInfo(const bool & f) {
    cout << "armour type: Armour ";
}


//----------Constant Armour----------

int ConstArmour::CalculateNewDamage(const int & damage) {
    return damage - reduction;
}

void ConstArmour::Read(ifstream & in) {
    in >> this->reduction;
}

void ConstArmour::Write(std::ofstream & out, const bool & f) const {
    out << CONST_ARMOUR_CHAR << " " << this->reduction << " ";
}

void ConstArmour::PrintInfo(const bool & f) {
    cout << "armour type: ConstArmour, retuction: " << this->reduction << " ";
}


//----------Proportional Armour----------

int ProportionalArmour::CalculateNewDamage(const int & damage) {
    return damage * this->multiplier;
}

void ProportionalArmour::Read(std::ifstream & in) {
    in >> this->multiplier;
}

void ProportionalArmour::Write(std::ofstream & out, const bool & f) const {
    out << PROPORTIONAL_ARMOUR_CHAR << " " << this->multiplier << " ";
}

void ProportionalArmour::PrintInfo(const bool & f) {
    cout << "armour type: ProportionalArmour, multiplier: " << this->multiplier << " ";
}



//----------Multi Armour----------

int MultiArmour::CalculateNewDamage(const int & damage) {
    int amount = this->subArmours.size();
    int newDamage = damage;
    for (int i = 0; i < amount; i++){
        newDamage = subArmours[i]->CalculateNewDamage(damage);
    }
    return newDamage;
}

void MultiArmour::Read(std::ifstream & in) {
    int amount;
    in >> amount;
    for (int i = 0; i < amount; i++){
        Armour* newArmour = Armour::GetNewArmour(in);
        subArmours.push_back(newArmour);
    }
}

void MultiArmour::Write(std::ofstream & out, const bool & f) const {
    out << MULTI_ARMOUR_CHAR << " " << this->subArmours.size() << " ";
    int amount = this->subArmours.size();
    for (int i = 0; i < amount; i++) this->subArmours[i]->Write(out);
}

void MultiArmour::PrintInfo(const bool & f) {
    cout << "armour type: MultiArmour, sub-armours amount: " << this->subArmours.size() << ", sub armours: ";
    int amount = this->subArmours.size();
    for (int i = 0; i < amount; i++){
        cout << "\n\t\t";
        this->subArmours[i]->PrintInfo();
    }
}

MultiArmour::~MultiArmour(){
    int amount = this->subArmours.size();
    for (int i = amount - 1; i >= 0; i--){
        Armour* a = subArmours[i];
        delete a;
        subArmours.pop_back();
    }
}


//----------Operators----------

ifstream& operator >> (ifstream & is, Armour & armour){
    armour.Read(is);
    return is;
}

ofstream& operator << (ofstream & os, const Armour & armour){
    armour.Write(os);
    return os;
}