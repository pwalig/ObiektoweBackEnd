#include "Being.hh"
#include "HPBeing.hpp"
#include "MoveAble.hpp"
#include "Knight.hh"
#include <fstream>

using std::ifstream;
using std::ofstream;
using std::string;
using std::vector;
using std::cout;
using std::endl;
using nlohmann::json;

#define BEING_CHAR 'b'


// ----------Being Static Methods----------

Being* Being::GetNewBeing(const char & type){
    switch(type)
    {
        case BEING_CHAR:
        return new Being();
        break;
        
    case HPBEING_CHAR:
        return new HPBeing();
        break;

        
    case MOVEABLE_CHAR:
        return new MoveAble();
        break;
    
    case KNIGHT_CHAR:
        return new Knight();
        break;
    
    default:
        return new Being();
        break;
    }
}

Being* Being::GetNewBeing(const json & data){
    int t;
    t = data["type"];
    Being *b = Being::GetNewBeing(t);
    b->Read(data);
    return b;
}

Being* Being::GetNewBeing(ifstream & in){
    char t;
    in >> t;
    Being *b = Being::GetNewBeing(t);
    b->Read(in);
    return b;
}


// ----------Being Common----------

int Being::GetX() const{
    return this->x;
}
int Being::GetY() const{
 
    return this->y;
}
int Being::GetOwner() const{
    return this->owner;
}

int Being::GetPriority() const{
    return this->priority;
}

void Being::SetX(const int val){
    this->x = val;
}
void Being::SetY(const int val){
    this->y = val;
}
void Being::SetOwner(const int val){
    this->owner = val;
}
void Being::SetPriority(const int val){
    this->priority = val;
}


void Being::SetGame(MainGame* mg){
    this->game = mg;
}


// ----------Being----------

void Being::Act() {}

void Being::Read(ifstream & in) {
    in >> x;
    in >> y;
    in >> priority;
}

void Being::Read(const json & data)
{
    x = data["x"];
    y = data["y"];
    owner = data["owner"];
    priority = data["priority"];
}

void Being::Write(ofstream & out, const bool & f) const {
    if (f) out << BEING_CHAR << " ";
    out << x << " " << y << " " << priority;
    if (f) out << endl;
    else out << " ";
}

json Being::Write(){
    json data;
    data["type"] = BEING_CHAR;
    data["x"]=this->GetX();
    data["y"]=this->GetY();
    data["owner"]=this->GetOwner();
    data["priority"]=this->GetPriority();
    return data;
}


void Being::PrintInfo(const bool & f){
    if (f) cout << "type: Being, ";
    cout << "x: " << this->x << ", y: " << this->y << ", priority: " << this->priority;
    if (f) cout << endl;
    else cout << ", ";
}


//----------Operators----------

ifstream& operator >> (ifstream & is, Being & being){
    being.Read(is);
    return is;
}

ofstream& operator << (ofstream & os, const Being & being){
    being.Write(os);
    return os;
}