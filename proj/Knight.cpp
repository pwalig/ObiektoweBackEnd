#include "Knight.hh"
#include "MoveAble.hpp"

#include <fstream>

using std::ifstream;
using std::ofstream;
using std::string;
using std::vector;
using std::cout;
using std::endl;
using nlohmann::json;


void Knight::Act()
{
    int max=0;
    int moveHor=0;
    int moveVer=0;
    for(int i=-2; i<3; i+=4)
    {
        if(x + i < game->GetBoard()->GetFieldsSize() && x + i >=0)
            for(int j=-1; j<2; j+=2)
            {
                if(y + j < game->GetBoard()->GetFieldsSize() && y + j >=0 && game->GetBoard()->GetBeing(x+i, y+j) != NULL)
                {
                    Being* bng = game->GetBoard()->GetBeing(x+i, y+j);
                    if(HPBeing* ptrHPBeing = dynamic_cast<HPBeing*>(bng))
                    {
                        if(ptrHPBeing->GetHP() > max)
                        {
                            max = ptrHPBeing->GetHP();
                            moveHor=i;
                            moveVer=j;
                        }
                    }
                }
            }
        if(y + i < game->GetBoard()->GetFieldsSize() && y + i >=0)
            for(int j=-1; j<2; j+=2)
            {
                if(x + j < game->GetBoard()->GetFieldsSize() && x + j >=0 && game->GetBoard()->GetBeing(x+j, y+i) != NULL)
                {
                    Being* bng = game->GetBoard()->GetBeing(x+j, y+i);
                    if(HPBeing* ptrHPBeing = dynamic_cast<HPBeing*>(bng))
                    {
                        if(ptrHPBeing->GetHP() > max)
                        {
                            max = ptrHPBeing->GetHP();
                            moveHor=j;
                            moveVer=i;
                        }
                    }
                }
            }
    }
    HPBeing* ptr = dynamic_cast<HPBeing*>(game->GetBoard()->GetBeing(x+moveHor, y+moveVer));
    if(DealDamage(ptr, baseDamage) <= 0)
    {
        int erCode = game->GetBoard()->Move(x, y, x+moveHor, y+moveVer);
        if (erCode == 0){
            x += moveHor;
            y += moveVer;
        }
    }    
}

int Knight::GetBaseDamage() const
{
    return this->baseDamage;
}

void Knight::SetBaseDamage(const int dam)
{
    this->baseDamage = dam;
}


void Knight::Read(ifstream & in) {
    this->HPBeing::Read(in);
    in >> this->baseDamage;
}

void Knight::Read(const json & data) {
    this->HPBeing::Read(data);
    this->baseDamage = data["baseDamage"];
}


void Knight::Write(ofstream & out, const bool & f) const {
    if (f) out << KNIGHT_CHAR << " ";
    this->HPBeing::Write(out, false);
    out << this->baseDamage<< " ";
    if (f) out << endl;
    else out << " ";
}

json Knight::Write(){
    json data;
    data["type"] = KNIGHT_CHAR;
    this->HPBeing::Write();
    data["baseDamage"] = baseDamage;
    return data;
}

void Knight::PrintInfo(const bool & f) {
    if (f) cout << "type: Knight, ";
    this->HPBeing::PrintInfo(false);
    cout << "baseDamage: " << this->baseDamage;
    if (f) cout << endl;
    else cout << ", ";
}