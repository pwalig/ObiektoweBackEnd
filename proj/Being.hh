#pragma once

#include <fstream>
using namespace std;

class Being{
    protected:
    int x;
    int y;

    virtual void Read(ifstream & _ifstream);

    public:
    virtual void Act();
    virtual void PrintInfo();
    
    static Being* GetPointerFromStream(ifstream & _ifstream);
};

class Living : public Being{
    protected:
    int hp = 0;
    virtual void Read(ifstream & _ifstream);

    public:
    virtual void Act();
    virtual void PrintInfo();
};

class BasicBeing : public Being{
    int value = 0;

    protected:
    virtual void Read(ifstream & _ifstream);

    public:
    virtual void Act();
    virtual void PrintInfo();
};