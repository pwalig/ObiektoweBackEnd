#include "Being.hh"
#include <fstream>

using std::ifstream;
using std::ofstream;
using std::string;
using std::vector;
using std::cout;
using std::endl;
using nlohmann::json;

#define BEING_CHAR 'b'
#define HPBEING_CHAR 'h'
#define TESTBEING_CHAR 't'


// ----------Being Static Methods----------

Being* Being::GetNewBeing(json data){
    int t;
    t = data["type"];
    Being *b;
    switch(t)
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
    b->Read(data);
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

int Being::partitionBeingsByPriority(vector<Being*> & beings, const int & start, const int & end, const bool & ascendingOrder)
{
    int startPivot = (start + end) / 2;
    int pivot = beings[startPivot]->GetPriority();
 
    int count = 0;
    for (int i = start; i <= end; i++) {
        if (i != startPivot && ((beings[i]->GetPriority() <= pivot && ascendingOrder) || (beings[i]->GetPriority() > pivot && !ascendingOrder)))
            count++;
    }
 
    // Giving pivot element its correct position
    int pivotIndex = start + count;
    std::swap(beings[pivotIndex], beings[startPivot]);
 
    // Sorting left and right parts of the pivot element
    int i = start, j = end;
 
    while (i < pivotIndex && j > pivotIndex) {
 
        while ((beings[i]->GetPriority() <= pivot && ascendingOrder) || (beings[i]->GetPriority() > pivot && !ascendingOrder)) {
            i++;
        }
 
        while ((beings[j]->GetPriority() > pivot && ascendingOrder) || (beings[j]->GetPriority() <= pivot && !ascendingOrder)) {
            j--;
        }
 
        if (i < pivotIndex && j > pivotIndex) {
            std::swap(beings[i], beings[j]);
            i++; j--;
        }
    }
 
    return pivotIndex;
}

void Being::quickSortBeingsByPriority(vector<Being*> & beings, const int & start, const int & end, const bool & ascendingOrder)
{
 
    // base case
    if (start >= end)
        return;
 
    // partitioning the array
    int p = partitionBeingsByPriority(beings, start, end, ascendingOrder);
 
    // Sorting the left part
    quickSortBeingsByPriority(beings, start, p - 1, ascendingOrder);
 
    // Sorting the right part
    quickSortBeingsByPriority(beings, p + 1, end, ascendingOrder);
}


// ----------Being Common----------

int Being::GetX(){
    return this->x;
}
int Being::GetY(){
 
    return this->y;
}
int Being::GetOwner(){
    return this->owner;
}

int Being::GetPriority(){
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

void Being::Read(json & data)
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


// ----------HPBeing----------

void HPBeing::Act() {}


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

void HPBeing::Read(json & data) {
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
    json data;
    data["type"] = HPBEING_CHAR;
    this->Being::Write();
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


// ----------TestBeing----------

void TestBeing::Act() {
    cout << "Basic Being!!! " << this-> value << endl;
}

void TestBeing::Read(ifstream & in) {
    this->Being::Read(in);
    in >> value;
}

void TestBeing::Read(json & data) {
    this->Being::Read(data);
    value = data["hp"];
}

void TestBeing::Write(ofstream & out, const bool & f) const {
    if (f) out << TESTBEING_CHAR << " ";
    this->Being::Write(out, false);
    out << value;
    if (f) out << endl;
    else out << " ";
}

json TestBeing::Write(){
    json data;
    data["type"] = TESTBEING_CHAR;
    this->Being::Write();
    data["value"] = value;
    return data;
}

void TestBeing::PrintInfo(const bool & f){
    if (f) cout << "type: TestBeing, ";
    this->Being::PrintInfo(false);
    cout << "value: " << this->value;
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