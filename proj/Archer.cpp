#include "Archer.hpp"
#include <fstream>

using std::ifstream;
using std::ofstream;
using std::string;
using std::vector;
using std::cout;
using std::endl;
using nlohmann::json;

void Archer::Act() {
    this->MoveAble::Act();
    int dirs = 0;
    Being * b = nullptr;
    do {
        Being* b = game->GetBoard()->GetBeing(x + range, y);
        if (b == nullptr) Being* b = game->GetBoard()->GetBeing(x, y + range);
        if (b == nullptr) Being* b = game->GetBoard()->GetBeing(x - range, y);
        if (b == nullptr) Being* b = game->GetBoard()->GetBeing(x, y - range);
        ++dirs;
    } while(b == nullptr && dirs < range);
    if (b != nullptr) {
        if(HPBeing* ptrHPBeing = dynamic_cast<HPBeing*>(b)) {
            HPBeing::DealDamage(ptrHPBeing, damage);
        }
    }
}


void Archer::Read(ifstream & in) {
    this->MoveAble::Read(in);
    in >> this->moveX;
    in >> this->moveY;
}

void Archer::Read(const json & data) {
    this->MoveAble::Read(data);
    this->damage = data["damage"];
    this->range = data["range"];
}


void Archer::Write(ofstream & out, const bool & f) const {
    if (f) out << MOVEABLE_CHAR << " ";
    this->MoveAble::Write(out, false);
    out << this->damage << " " << this->range;
    if (f) out << endl;
    else out << " ";
}

json Archer::Write(){
    json data = MoveAble::Write();
    data["type"] = MOVEABLE_CHAR;
    data["damage"] = damage;
    data["range"] = range;
    return data;
}

void Archer::PrintInfo(const bool & f) {
    if (f) cout << "type: Archer, ";
    this->MoveAble::PrintInfo(false);
    cout << "damage: " << this->damage << ", range: " << this->range;
    if (f) cout << endl;
    else cout << ", ";
}