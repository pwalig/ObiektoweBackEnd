#pragma once

class Being{
    public:
    virtual void Act();
};

class BasicBeing : public Being{
    public:
    virtual void Act();
};