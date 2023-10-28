#include "AiModel.hh"

int AiModel::GetDecision(const int & _inputSize, float * _inputs, const int & requestNo) {
    float * decisions = this->neuralNetwork.EvaluateGetDecision(_inputSize, _inputs);
    return sortedDecisions[requestNo];
}

/* int AiModel::GetDecision(const int & _inputSize, int * _inputs, const int & requestNo) {
    float * fdecisions = new float [_inputSize];
    for (int i = 0; i < _inputSize; i++) {
        fdecisions[i] = _inputs[i];
    }
    float * decisions = this->neuralNetwork.EvaluateGetDecision(_inputSize, fd);
}*/