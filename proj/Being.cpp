#include "Being.hh"
#include <iostream>

using namespace std;

#define BEING_CHAR 'b'
#define HPBEING_CHAR 'h'
#define TESTBEING_CHAR 't'


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
    case BEING_CHAR:
        b = new Being();
        break;
        
    case TESTBEING_CHAR:
        b = new TestBeing();
        break;
        
    case HPBEING_CHAR:
        b = new HPBeing();
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
    if (f) out << BEING_CHAR << " ";
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


// ----------HPBeing----------

void HPBeing::Act() {}


void HPBeing::DealDamage(const int & damage){
    hp -= damage;
    if (hp <= 0) this->game->Destroy(this);
}

int HPBeing::DealDamage(HPBeing* hpb, const int & damage){
    hpb->hp -= damage;
    int out = hpb->hp;
    if (hpb->hp <= 0) hpb->game->Destroy(hpb);
    return out;
}

void HPBeing::Read(ifstream & in) {
    this->Being::Read(in);
    in >> hp;
}
void HPBeing::Write(ofstream & out, const bool & f) {
    if (f) out << HPBEING_CHAR << " ";
    this->Being::Write(out, false);
    out << hp;
    if (f) out << endl;
    else out << " ";
}

void HPBeing::PrintInfo(const bool & f) {
    if (f) cout << "type: HPBeing, ";
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
    if (f) out << TESTBEING_CHAR << " ";
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