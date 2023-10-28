#pragma once

#include "player.hh"
#include "Neural.hh"

class AiModel : public Player{
    private:
    NeuralNetwork neuralNetwork;
    int * sortedDecisions;
    public:
    virtual int GetDecision(const int & _inputSize, float * inputs, const int & requestNo);
    virtual int GetDecision(const int & _inputSize, int * inputs, const int & requestNo);
};