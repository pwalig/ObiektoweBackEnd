#pragma once

class Player
{
    public:
    bool requireDisplay = false;
    static float* ConvertIntToFloat(const int & _size, int * _values, const bool & normalize);
    virtual int GetDecision(const int & _inputSize, float * _inputs, const int & requestNo);
};

class HumanPlayer : public Player
{
    public:
    HumanPlayer();
    virtual int GetDecision(const int & _inputSize, float * _inputs, const int & requestNo);
};

class RandomPlayer : public Player
{
    public:
    RandomPlayer();
    virtual int GetDecision(const int & _inputSize, float * _inputs, const int & requestNo);
};