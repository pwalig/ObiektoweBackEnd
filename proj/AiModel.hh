#pragma once

#include "player.hh"
#include "Neural.hh"

class AiModel : public Player {
    private:
    NeuralNetwork neuralNetwork;
    int * sortedDecisions;
    public:
    AiModel(const int & _layers, int* _layerSizes);
    AiModel(const NeuralNetwork & _neuralNetwork);
    NeuralNetwork* GetNeuralNetwork();
    virtual int GetDecision(const int & _inputSize, float * _inputs, const int & requestNo);
};