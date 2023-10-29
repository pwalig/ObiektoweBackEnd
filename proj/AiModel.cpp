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

void AiModelTest(){
    int layers = 3;
    int layerSiz[3] = {3, 3, 5};
    AiModel am(layers, layerSiz);
    am.GetNeuralNetwork()->RandomizeNetwork();
    
    float inputs[3] = {0.3, 0.8, 0.1};
    int dec = am.GetDecision(layers, inputs, 0);
    am.GetNeuralNetwork()->PrintInfo();

    printf("Decision no 1: %d\n", dec);
    dec = am.GetDecision(layers, inputs, 1);
    printf("Decision no 2: %d\n", dec);
    dec = am.GetDecision(layers, inputs, 2);
    printf("Decision no 3: %d\n", dec);
    dec = am.GetDecision(layers, inputs, 3);
    printf("Decision no 4: %d\n", dec);
    dec = am.GetDecision(layers, inputs, 4);
    printf("Decision no 5: %d\n", dec);

}