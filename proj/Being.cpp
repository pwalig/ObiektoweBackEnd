#include "Being.hh"
#include <iostream>

using namespace std;


Being* Being::GetPointerFromStream(ifstream & _ifstream){
    int t;
    _ifstream >> t;
    Being* b;
    switch (t)
    {
    case 0:
        b = new Being();
        break;
        
    case 1:
        b = new BasicBeing();
        break;
        
    case 2:
        b = new Living();
        break;
    
    default:
        break;
    }
    b->Read(_ifstream);
    return b;
}


// ----------Being----------

void Being::Act() {}

void Being::Read(ifstream & _ifstream) {
    _ifstream >> x;
    _ifstream >> y;
}

void Being::PrintInfo(){
    cout << "x: " << this->x << ", y: " << this->y << endl;
}


// ----------Living----------

void Living::Act() {}

void Living::Read(ifstream & _ifstream) {
    this->Being::Read(_ifstream);
    _ifstream >> hp;
}

void Living::PrintInfo() {
    cout << "hp: " << this->hp << ", ";
    this->Being::PrintInfo();
}


// ----------BasicBeing----------

void BasicBeing::Act() {
    cout << "Basic Being!!!" << this-> value << endl;
}

void BasicBeing::Read(ifstream & _ifstream) {
    this->Being::Read(_ifstream);
    _ifstream >> value;
}

void BasicBeing::PrintInfo(){
    cout << "value: " << this->value << ", ";
    this->Being::PrintInfo();
}