#include "HPBeing.hpp"
#include <fstream>

using std::ifstream;
using std::ofstream;
using std::string;
using std::vector;
using std::cout;
using std::endl;
using nlohmann::json;

void HPBeing::Act() {}

int HPBeing::GetHP() const
{
    return this->hp;
}

void HPBeing::DealDamage(const int & damage){
    hp -= armour->CalculateNewDamage(damage);
    if (hp <= 0) this->game->Destroy(this);
}

int HPBeing::DealDamage(HPBeing* hpb, const int & damage){
    hpb->hp -= hpb->armour->CalculateNewDamage(damage);
    int out = hpb->hp;
    if (hpb->hp <= 0) hpb->game->Destroy(hpb);
    return out;
}

void HPBeing::Read(ifstream & in) {
    this->Being::Read(in);
    in >> hp;
    armour = Armour::GetNewArmour(in);
}

void HPBeing::Read(const json & data) {
    this->Being::Read(data);
    hp = data["hp"];
    armour = Armour::GetNewArmour(data["armour"]);
}

void HPBeing::Write(ofstream & out, const bool & f) const {
    if (f) out << HPBEING_CHAR << " ";
    this->Being::Write(out, false);
    out << hp;
    armour->Write(out);
    if (f) out << endl;
    else out << " ";
}

json HPBeing::Write(){
    json data = Being::Write();
    data["type"] = HPBEING_CHAR;
    data["hp"] = hp;
    data["armour"] = armour->Write();
    return data;
}

void HPBeing::PrintInfo(const bool & f) {
    if (f) cout << "type: HPBeing, ";
    this->Being::PrintInfo(false);
    cout << "hp: " << this->hp << ", ";
    this->armour->PrintInfo();
    if (f) cout << endl;
    else cout << ", ";
}

HPBeing::~HPBeing(){
    delete armour;
}