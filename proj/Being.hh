#pragma once

class Being{
    public:
    virtual void Act();
};

class Living : public Being{
    int hp = 0;
    public:
    virtual void Act();
};

class BasicBeing : public Being{
    public:
    virtual void Act();
};