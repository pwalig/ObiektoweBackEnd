#include "Being.hh"
#include <iostream>

using namespace std;


// ----------Being Static Methods----------

Being* GetNewBeing(string filename){
    ifstream in;
    in.open(filename);
    Being* b = Being::GetNewBeing(in);
    in.close();
    return b;
}

Being* Being::GetNewBeing(ifstream & in){
    char t;
    in >> t;
    Being* b;
    switch (t)
    {
    case 'b':
        b = new Being();
        break;
        
    case 't':
        b = new TestBeing();
        break;
        
    case 'l':
        b = new Living();
        break;
    
    default:
        b = new Being();
        break;
    }
    b->Read(in);
    return b;
}


// ----------Being Common----------

int Being::GetX(){
    return this->x;
}
int Being::GetY(){
    return this->y;
}
int Being::GetPriority(){
    return this->priority;
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
void Being::Write(ofstream & out, const bool & f) {
    if (f) out << "b ";
    out << x << " " << y << " " << priority;
    if (f) out << endl;
    else out << " ";
}

void Being::PrintInfo(const bool & f){
    if (f) cout << "type: Being, ";
    cout << "x: " << this->x << ", y: " << this->y << ", priority: " << this->priority;
    if (f) cout << endl;
    else cout << ", ";
}


// ----------Living----------

void Living::Act() {}

void Living::Read(ifstream & in) {
    this->Being::Read(in);
    in >> hp;
}
void Living::Write(ofstream & out, const bool & f) {
    if (f) out << "l ";
    this->Being::Write(out, false);
    out << hp;
    if (f) out << endl;
    else out << " ";
}

void Living::PrintInfo(const bool & f) {
    if (f) cout << "type: Living, ";
    this->Being::PrintInfo(false);
    cout << "hp: " << this->hp;
    if (f) cout << endl;
    else cout << ", ";
}


// ----------TestBeing----------

void TestBeing::Act() {
    cout << "Basic Being!!! " << this-> value << endl;
}

void TestBeing::Read(ifstream & in) {
    this->Being::Read(in);
    in >> value;
}
void TestBeing::Write(ofstream & out, const bool & f) {
    if (f) out << "t ";
    this->Being::Write(out, false);
    out << value;
    if (f) out << endl;
    else out << " ";
}

void TestBeing::PrintInfo(const bool & f){
    if (f) cout << "type: TestBeing, ";
    this->Being::PrintInfo(false);
    cout << "value: " << this->value;
    if (f) cout << endl;
    else cout << ", ";
}