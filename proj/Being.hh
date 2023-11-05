#pragma once

class Being{
    int hp;
    public:
    virtual void Act();
};

class BasicBeing : public Being{
    public:
    virtual void Act();
};