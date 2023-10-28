#pragma once

#include "player.hh"
#include "Neural.hh"

class AiModel : public Player{
    private:
    NeuralNetwork neuralNetwork;
    int * sortedDecisions;
    public:
    AiModel(const NeuralNetwork & _neuralNetwork);
    virtual int GetDecision(const int & _inputSize, float * _inputs, const int & requestNo);
};