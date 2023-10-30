#include "AiModel.hh"
#include <stdio.h>

AiModel::AiModel(const int & _layers, int* _layerSizes):neuralNetwork(_layers, _layerSizes), sortedDecisions(nullptr){
    this->requireDisplay = false;
}

AiModel::AiModel(const NeuralNetwork & _neuralNetwork):neuralNetwork(_neuralNetwork), sortedDecisions(nullptr){
    this->requireDisplay = false;
}

NeuralNetwork* AiModel::GetNeuralNetwork(){
    return &(this->neuralNetwork);
}

int AiModel::GetDecision(const int & _inputSize, float * _inputs, const int & requestNo) {
    if (requestNo == 0) {
        return neuralNetwork.EvaluateGetDecision(_inputSize, _inputs);
    }
    if (requestNo == 1) {
        if (sortedDecisions) delete [] this->sortedDecisions;
        this->sortedDecisions = neuralNetwork.GetSortedDecisions();
    }
    if (requestNo < neuralNetwork.GetOutputsAmount()) {
        return sortedDecisions[requestNo];
    }
    return requestNo;
}