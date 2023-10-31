#pragma once

#include "player.hh"
#include "Neural.hh"

class AiModel : public Player {
    private:
    NeuralNetwork neuralNetwork;
    int * sortedDecisions;
    public:
    AiModel();
    AiModel(const int & _layers, int* _layerSizes);
    AiModel(const NeuralNetwork & _neuralNetwork);
    AiModel(const AiModel & _aiModel);
    NeuralNetwork* GetNeuralNetwork();
    virtual int GetDecision(const int & _inputSize, float * _inputs, const int & requestNo);
    ~AiModel();
};