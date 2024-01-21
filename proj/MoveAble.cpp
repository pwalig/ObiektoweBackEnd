#include "MoveAble.hpp"
#include <fstream>

using std::ifstream;
using std::ofstream;
using std::string;
using std::vector;
using std::cout;
using std::endl;
using nlohmann::json;

void MoveAble::Act() {
    int erCode = game->GetBoard()->Move(x, y, x+moveX, y+moveY);
    if (erCode == 0){
        x += moveX;
        y += moveY;
    }
    this->HPBeing::Act();
}


void MoveAble::Read(ifstream & in) {
    this->HPBeing::Read(in);
    in >> this->moveX;
    in >> this->moveY;
}

void MoveAble::Read(const json & data) {
    this->HPBeing::Read(data);
    this->moveX = data["moveX"];
    this->moveY = data["moveY"];
}


void MoveAble::Write(ofstream & out, const bool & f) const {
    if (f) out << MOVEABLE_CHAR << " ";
    this->HPBeing::Write(out, false);
    out << this->moveX << " " << this->moveY;
    if (f) out << endl;
    else out << " ";
}

json MoveAble::Write(){
    json data = HPBeing::Write();
    data["type"] = MOVEABLE_CHAR;
    data["moveX"] = moveX;
    data["moveY"] = moveY;
    return data;
}

void MoveAble::PrintInfo(const bool & f) {
    if (f) cout << "type: MoveAble, ";
    this->HPBeing::PrintInfo(false);
    cout << "moveX: " << this->moveX << ", moveY: " << this->moveY;
    if (f) cout << endl;
    else cout << ", ";
}