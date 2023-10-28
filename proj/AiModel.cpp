#include "AiModel.hh"

AiModel::AiModel(const NeuralNetwork & _neuralNetwork){
    this->neuralNetwork = _neuralNetwork;
}

// method under consturction
int AiModel::GetDecision(const int & _inputSize, float * _inputs, const int & requestNo) {
    float * decisions = this->neuralNetwork.EvaluateGetDecision(_inputSize, _inputs);
    delete [] decisions;
    return sortedDecisions[requestNo];
}