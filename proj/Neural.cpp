#include "Neural.hh"

#include <cmath>
#include <stdio.h>

// UTILS

float FastSigmoid(const float & x){
    return 0.5 * (x / (1 + std::abs(x))) + 0.5;
}

float RanodmFloat(const float & _min, const float & _max){
    if (_min == _max) return _min;
    return _min + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(_max-_min)));
}

float* RandomList(const int & _size, const float & _min, const float & _max){
    float* out = new float [_size];
    for (int i = 0; i < _size; i++){
        out[i] = RanodmFloat(_min, _max);
    }
    return out;
}


// NEURON

Neuron::Neuron():activation(0), bias(0), connectionsCount(0), connections(nullptr){}

Neuron::Neuron(const int & _connectionsCount, Neuron* _inputNeurons):activation(0), bias(0), connectionsCount(_connectionsCount){
    this->connections = new WeightedConnection[_connectionsCount];
    for (int i = 0; i < _connectionsCount; i++)
    {
        this->connections[i].neuron = &_inputNeurons[i];
    }
}


Neuron::Neuron(const Neuron & _neuron):activation(_neuron.activation), bias(_neuron.bias), connectionsCount(_neuron.connectionsCount){
    this->connections = new WeightedConnection[_neuron.connectionsCount];
    for (int i = 0; i < _neuron.connectionsCount; i++) {
        this->connections[i].neuron = _neuron.connections[i].neuron;
        this->connections[i].weight = _neuron.connections[i].weight;
    }
}

void Neuron::CreateConnections(const int & _connectionsCount, Neuron* _inputNeurons){
    if (this->connections != nullptr){
        delete [] this->connections;
    }
    this->connections = new WeightedConnection[_connectionsCount];
    for (int i = 0; i < _connectionsCount; i++)
    {
        this->connections[i].neuron = &_inputNeurons[i];
    }
    this->connectionsCount = _connectionsCount;
}

void Neuron::CreateConnections(const int & _connectionsCount, Neuron* _inputNeurons, float* _weights){
    if (this->connections != nullptr){
        delete [] this->connections;
    }
    this->connections = new WeightedConnection[_connectionsCount];
    for (int i = 0; i < _connectionsCount; i++)
    {
        this->connections[i].neuron = &_inputNeurons[i];
        this->connections[i].weight = _weights[i];
    }
    this->connectionsCount = _connectionsCount;
}

int Neuron::GetConnectionsCount(){
    return this->connectionsCount;
}

float Neuron::GetActivation(){
    return this->activation;
}

void Neuron::SetActivation(const float & _activation){
    this->activation = _activation;
}


float Neuron::GetBias(){
    return this->bias;
}

void Neuron::SetWeights(const int & _size, float* _weights){
    for (int i = 0; i < connectionsCount && i <_size; i++){
        this->connections[i].weight = _weights[i];
    }
}

float* Neuron::GetWeights(){
    float* out = new float[this->connectionsCount];
    for (int i = 0; i < connectionsCount; i++){
        out[i] = this->connections[i].weight;
    }
    return out;
}

void Neuron::SetBias(const float & _bias){
    this->bias = _bias;
}

float Neuron::EvaluateActivation(){
    float wSum = 0.0;
    for (int i = 0; i < this->connectionsCount; i++)
        wSum += this->connections->neuron->GetActivation() * this->connections->weight;
    wSum += bias;
    wSum = FastSigmoid(wSum);
    this->activation = wSum;
    return this->activation;
}

void Neuron::PrintInfo(){
    printf("Activation: %f | Bias: %f | Connections: %d\n", activation, bias, connectionsCount);
    for(int i = 0; i < connectionsCount; i++){
        printf("--Connection %d: Wieght: %f\n", i, connections[i].weight);
    }
}

Neuron::~Neuron(){
    if (connections){
        delete [] connections;
    }
}


// NEURAL LAYER

void NeuralLayer::Initialise(const int & _neuronCount){
    if (this->neurons != nullptr)
        delete [] this->neurons;
    this->neurons = new Neuron[_neuronCount];
}

NeuralLayer::NeuralLayer(const int & _neuronCount){
    this->neurons = new Neuron[_neuronCount];
    this->neuronCount = _neuronCount;
}


// NEURAL NETWORK

NeuralNetwork::NeuralNetwork():neurons(nullptr), layers(0), layerSizes(nullptr){}

NeuralNetwork::NeuralNetwork(const int & _layers, int* _layerSizes):layers(_layers){
    this->neurons = new Neuron*[_layers];
    this->layerSizes = new int[_layers];
    for (int i = 0; i <_layers; i++){
        this->neurons[i] = new Neuron[_layerSizes[i]];
        this->layerSizes[i] = _layerSizes[i];
        for (int j = 0; i > 0 && j < _layerSizes[i]; j++){
            this->neurons[i][j].CreateConnections(_layerSizes[i-1], this->neurons[i-1]);
        }
    }
}

NeuralNetwork::NeuralNetwork(const NeuralNetwork & _neuralNetwork):layers(_neuralNetwork.layers){
    this->layerSizes = new int [_neuralNetwork.layers];
    this->neurons = new Neuron* [_neuralNetwork.layers];
    for (int i = 0; i < _neuralNetwork.layers; i++){
        this->layerSizes[i] = _neuralNetwork.layerSizes[i];
        this->neurons[i] = new Neuron [_neuralNetwork.layerSizes[i]];
        if (i==0) continue;
        for (int j = 0; j < _neuralNetwork.layerSizes[i]; j++){
            float* _weights = _neuralNetwork.neurons[i][j].GetWeights();
            this->neurons[i][j].SetBias(_neuralNetwork.neurons[i][j].GetBias());
            this->neurons[i][j].SetActivation(_neuralNetwork.neurons[i][j].GetActivation());
            this->neurons[i][j].CreateConnections(this->layerSizes[i-1], this->neurons[i-1], _weights);
            delete [] _weights;
        }
    }
}

void NeuralNetwork::RandomizeLayerWeights(const int & layerId){
    if (layerId == 0) return;
    for (int i = 0; i < layerSizes[layerId]; i++){
        float* _weights = RandomList(layerSizes[layerId-1], -5.0, 5.0);
        neurons[layerId][i].SetWeights(layerSizes[layerId-1], _weights);
        delete [] _weights;
    }
}
void NeuralNetwork::RandomizeLayerBiases(const int & layerId){
    if (layerId == 0) return;
    for (int i = 0; i < layerSizes[layerId]; i++){
        float _bias = RanodmFloat(-10.0, 10.0);
        neurons[layerId][i].SetBias(_bias);
    }
}
void NeuralNetwork::RandomizeLayer(const int & layerId){
    if (layerId == 0) return;
    for (int i = 0; i < layerSizes[layerId]; i++){
        float* _weights = RandomList(layerSizes[layerId-1], -5.0, 5.0);
        neurons[layerId][i].SetWeights(layerSizes[layerId-1], _weights);
        delete [] _weights;
        float _bias = RanodmFloat(-10.0, 10.0);
        neurons[layerId][i].SetBias(_bias);
    }
}
void NeuralNetwork::RandomizeNetworkWeights(){
    for (int i = 1; i < layers; i++){
        RandomizeLayerWeights(i);
    }
}
void NeuralNetwork::RandomizeNetworkBiases(){
    for (int i = 1; i < layers; i++){
        RandomizeLayerBiases(i);
    }
}
void NeuralNetwork::RandomizeNetwork(){
    for (int i = 1; i < layers; i++){
        RandomizeLayer(i);
    }
}

void NeuralNetwork::SetInputs(const int & _count, float* _inputs){
    for (int i = 0; i < _count && i < this->layerSizes[0]; i++) {
        float v = _inputs[i];
        this->neurons[0][i].SetActivation(v);
    }
}

void NeuralNetwork::EvaluateLayer(const int & layerId){
    for (int i = 0; i < layerSizes[layerId]; i++){
        neurons[layerId][i].EvaluateActivation();
    }
}

void NeuralNetwork::EvaluateNetwork(){
    for (int i = 1; i < layers; i++){
        EvaluateLayer(i);
    }
}

void NeuralNetwork::EvaluateNetwork(const int & _count, float* _inputs){
    SetInputs(_count, _inputs);
    EvaluateNetwork();
}

float* NeuralNetwork::GetDecision(){
    float* out = new float[layerSizes[layers-1]];
    for (int i = 0; i < layerSizes[layers-1]; i++){
        out[i] = neurons[layers-1][i].GetActivation();
    }
    return out;
}

float* NeuralNetwork::EvaluateGetDecision(){
    EvaluateNetwork();
    return GetDecision();
}

float* NeuralNetwork::EvaluateGetDecision(const int & _count, float* _inputs){
    SetInputs(_count, _inputs);
    EvaluateNetwork();
    return GetDecision();
}

void NeuralNetwork::PrintInfo(){
    printf("NeuralLayer Info:\nLayers: %d\n", layers);
    for (int i = 0; i < layers; i++){
        printf("--Layer %d: Neurons: %d\n", i, layerSizes[i]);
        for (int j = 0; j < layerSizes[i]; j++){
            printf("----Neuron %d: ", j);
            neurons[i][j].PrintInfo();
        }
    }
}

NeuralNetwork::~NeuralNetwork(){
    for (int i = 0; i < this->layers; i++){
        delete [] this->neurons[i];
    }
    delete [] this->neurons;
    delete [] this->layerSizes;
}

// TESTS
void NeuronTest(){
    printf("Running Neuron Test:\n");
    Neuron n;
    n.SetBias(15);
    n.PrintInfo();

    Neuron n2 = n;
    n2.PrintInfo();
}

void NeuralNetworkTest(){
    printf("Running NeuralNetwork Test:\n");
    int layers = 3;
    int layerSiz[3] = {3, 3, 3};
    NeuralNetwork nn(layers, layerSiz);
    nn.RandomizeNetwork();
    
    float inputs[3] = {0.3, 0.8, 0.1};
    nn.EvaluateNetwork(layers, inputs);
    nn.PrintInfo();

    NeuralNetwork nn2 = nn;
    nn2.PrintInfo();
    printf("Test finished");
}