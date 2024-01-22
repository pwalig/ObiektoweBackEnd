#include "Fighter.hpp"
#include <fstream>

using std::ifstream;
using std::ofstream;
using std::string;
using std::vector;
using std::cout;
using std::endl;
using nlohmann::json;

void Fighter::Act() {
    int max = 0;
    int attackX=0;
    int attackY=0;
    this->MoveAble::Act();
    int dirs = 0;
    Being * b = nullptr;
    for(int i=-1; i<2; i++)
    {
        for(int j=-1; j<2; j++)
        {
            if(x+i >=0 && x+i < game->GetBoard()->GetFieldsSize() && y+j >=0 && y < game->GetBoard()->GetFieldsSize())
            {
                Being* bng = game->GetBoard()->GetBeing(x+i, y+j);
                    if(HPBeing* ptrHPBeing = dynamic_cast<HPBeing*>(bng))
                    {
                        if(ptrHPBeing->GetOwner() != this->owner && ptrHPBeing->GetHP() > max)
                        {
                            max = ptrHPBeing->GetHP();
                            attackX=i;
                            attackY=j;
                        }
                    }
            }
        }
    }
    if(attackX != 0 && attackY != 0)
    {
        HPBeing* ptr = dynamic_cast<HPBeing*>(game->GetBoard()->GetBeing(x+attackX, y+attackY));
        DealDamage(ptr, damage);
    }
}


void Fighter::Read(ifstream & in) {
    this->MoveAble::Read(in);
    in >> this->moveX;
    in >> this->moveY;
}

void Fighter::Read(const json & data) {
    this->MoveAble::Read(data);
    this->damage = data["damage"];
}


void Fighter::Write(ofstream & out, const bool & f) const {
    if (f) out << FIGHTER_CHAR << " ";
    this->MoveAble::Write(out, false);
    out << this->damage;
    if (f) out << endl;
    else out << " ";
}

json Fighter::Write(){
    json data = MoveAble::Write();
    data["type"] = FIGHTER_CHAR;
    data["damage"] = damage;
    return data;
}

void Fighter::PrintInfo(const bool & f) {
    if (f) cout << "type: Fighter, ";
    this->MoveAble::PrintInfo(false);
    cout << "damage: " << this->damage;
    if (f) cout << endl;
    else cout << ", ";
}