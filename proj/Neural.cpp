#include "Neural.hh"

#include <cmath>
#include <stdio.h>

// UTILS

float FastSigmoid(const float & x){
    return 0.5 * (x / (1 + std::abs(x))) + 0.5;
}

float RandomFloat(const float & _min, const float & _max){
    if (_min == _max) return _min;
    return _min + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(_max-_min)));
}

float* RandomList(const int & _size, const float & _min, const float & _max){
    float* out = new float [_size];
    for (int i = 0; i < _size; i++){
        out[i] = RandomFloat(_min, _max);
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
        this->connections[i].weight = 0.0;
    }
}

Neuron::Neuron(const int & _connectionsCount, Neuron* _inputNeurons, float* _weights):activation(0), bias(0), connectionsCount(_connectionsCount){
    this->connections = new WeightedConnection[_connectionsCount];
    for (int i = 0; i < _connectionsCount; i++)
    {
        this->connections[i].neuron = &_inputNeurons[i];
        this->connections[i].weight = _weights[i];
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

void Neuron::VaryWeights(const int & _size, float* _weightVariances){
    for (int i = 0; i < connectionsCount && i <_size; i++){
        this->connections[i].weight += _weightVariances[i];
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

void Neuron::PrintInfo(const bool & _weights){
    printf("Activation: %f | Bias: %f | Connections: %d\n", activation, bias, connectionsCount);
    if (_weights) {
        for(int i = 0; i < connectionsCount; i++){
            printf("--Connection %d: Wieght: %f\n", i, connections[i].weight);
        }
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
            this->neurons[i][j].bias = _neuralNetwork.neurons[i][j].bias;
            this->neurons[i][j].activation = _neuralNetwork.neurons[i][j].activation;
            this->neurons[i][j].CreateConnections(this->layerSizes[i-1], this->neurons[i-1], _weights);
            delete [] _weights;
        }
    }
}


int NeuralNetwork::GetLayersAmount(){
    return this->layers;
}

int* NeuralNetwork::GetLayerSizes(){
    int * out = new int[layers];
    for (int i = 0; i < layers; i++) {
        out[i] = layerSizes[i];
    }
    return out;
}

int NeuralNetwork::GetInputsAmount(){
    return layerSizes[0];
}

int NeuralNetwork::GetOutputsAmount(){
    return layerSizes[layers-1];
}

void NeuralNetwork::RandomizeLayerWeights(const int & layerId, const float & _weightVariance){
    if (layerId == 0) return;
    for (int i = 0; i < layerSizes[layerId]; i++){
        float* _weights = RandomList(layerSizes[layerId-1], -_weightVariance, _weightVariance);
        neurons[layerId][i].SetWeights(layerSizes[layerId-1], _weights);
        delete [] _weights;
    }
}
void NeuralNetwork::RandomizeLayerBiases(const int & layerId, const float & _biasVariance){
    if (layerId == 0) return;
    for (int i = 0; i < layerSizes[layerId]; i++){
        float _bias = RandomFloat(-_biasVariance, _biasVariance);
        neurons[layerId][i].SetBias(_bias);
    }
}
void NeuralNetwork::RandomizeLayer(const int & layerId, const float & _weightVariance, const float & _biasVariance){
    if (layerId == 0) return;
    for (int i = 0; i < layerSizes[layerId]; i++){
        float* _weights = RandomList(layerSizes[layerId-1], -_biasVariance, _biasVariance);
        neurons[layerId][i].SetWeights(layerSizes[layerId-1], _weights);
        delete [] _weights;
        float _bias = RandomFloat(-_weightVariance, _weightVariance);
        neurons[layerId][i].SetBias(_bias);
    }
}
void NeuralNetwork::RandomizeNetworkWeights(const float & _weightVariance){
    for (int i = 1; i < layers; i++){
        RandomizeLayerWeights(i, _weightVariance);
    }
}
void NeuralNetwork::RandomizeNetworkBiases(const float & _biasVariance){
    for (int i = 1; i < layers; i++){
        RandomizeLayerBiases(i, _biasVariance);
    }
}
void NeuralNetwork::RandomizeNetwork(const float & _weightVariance, const float & _biasVariance){
    for (int i = 1; i < layers; i++){
        RandomizeLayer(i, _weightVariance, _biasVariance);
    }
}

void NeuralNetwork::VaryLayerWeights(const int & layerId, const float & _weightVariance){
    if (layerId == 0) return;
    for (int i = 0; i < layerSizes[layerId]; i++){
        float* _weights = RandomList(layerSizes[layerId-1], -_weightVariance, _weightVariance);
        neurons[layerId][i].VaryWeights(layerSizes[layerId-1], _weights);
        delete [] _weights;
    }
}
void NeuralNetwork::VaryLayerBiases(const int & layerId, const float & _biasVariance){
    if (layerId == 0) return;
    for (int i = 0; i < layerSizes[layerId]; i++){
        float _bias = RandomFloat(-_biasVariance, _biasVariance);
        neurons[layerId][i].bias += _bias;
    }
}
void NeuralNetwork::VaryLayer(const int & layerId, const float & _weightVariance, const float & _biasVariance){
    if (layerId == 0) return;
    for (int i = 0; i < layerSizes[layerId]; i++){
        float* _weights = RandomList(layerSizes[layerId-1], -_biasVariance, _biasVariance);
        neurons[layerId][i].VaryWeights(layerSizes[layerId-1], _weights);
        delete [] _weights;
        float _bias = RandomFloat(-_weightVariance, _weightVariance);
        neurons[layerId][i].bias += _bias;
    }
}
void NeuralNetwork::VaryNetworkWeights(const float & _weightVariance){
    for (int i = 1; i < layers; i++){
        VaryLayerWeights(i, _weightVariance);
    }
}
void NeuralNetwork::VaryNetworkBiases(const float & _biasVariance){
    for (int i = 1; i < layers; i++){
        VaryLayerBiases(i, _biasVariance);
    }
}
void NeuralNetwork::VaryNetwork(const float & _weightVariance, const float & _biasVariance){
    for (int i = 1; i < layers; i++){
        VaryLayer(i, _weightVariance, _biasVariance);
    }
}

NeuralNetwork NeuralNetwork::Breed(const int & _parentsCount, NeuralNetwork* _parents, float* _parentInfluences, const float & variance) {
    if (_parentsCount == 0) return NeuralNetwork(); // if no parents return empty network
    if (_parentsCount == 1) return _parents[0]; // if only one parent -> child should be identical
    NeuralNetwork child(_parents[0]);
    for (int i = 1; i < child.layers; i++){
        for (int j = 0; j < child.layerSizes[i]; j++){
            // prepare values
            float* _weights = new float[child.layerSizes[i-1]];
            float* _weightSums = new float[child.layerSizes[i-1]];
            for (int l = 0; l < child.layerSizes[i-1]; l++) _weights[l] = 0.0;
            for (int l = 0; l < child.layerSizes[i-1]; l++) _weightSums[l] = 0.0;
            float _bias = 0;
            float _biasSum = 0.0;

            for (int k = 0; k < _parentsCount; k++){
                float* _weights2 = _parents[k].neurons[i][j].GetWeights();
                float _bias2 = _parents[k].neurons[i][j].GetBias();

                if (variance <= 0.0){ // average breed
                    for (int l = 0; l < child.layerSizes[i-1]; l++){
                        _weights[l] += _weights2[l] * _parentInfluences[k];
                        _weightSums[l] += _parentInfluences[k];
                    }
                    _bias += _bias2 * _parentInfluences[k];
                    _biasSum += _parentInfluences[k];
                }

                else if (variance <= 1.0){ // interpolate breed
                    for (int l = 0; l < child.layerSizes[i-1]; l++){
                        float rf = RandomFloat(1.0 - variance, 1.0) * _parentInfluences[k];
                        _weights[l] += _weights2[l] * rf;
                        _weightSums[l] += rf;
                    }
                    float rf = RandomFloat(1.0 - variance, 1.0) * _parentInfluences[k];
                    _bias += _bias2 * rf;
                    _biasSum += rf;
                }

                else { // binary breed
                    for (int l = 0; l < child.layerSizes[i-1]; l++){
                        float rf = RandomFloat(0.0, 1.0);
                        if (rf < 1.0 - _parentInfluences[k]) rf /= variance;
                        else rf = 1.0 - ((1.0 - rf) / variance);
                        _weights[l] += _weights2[l] * rf;
                        _weightSums[l] += rf;
                    }

                    float rf = RandomFloat(0.0, 1.0);
                    if (rf < 1.0 - _parentInfluences[k]) rf /= variance;
                    else rf = 1.0 - ((1.0 - rf) / variance);
                    _bias += _bias2 * rf;
                    _biasSum += rf;
                }

                // prevent near 0 weight and bias sums, to avoid rounding errors
                if (_biasSum < 1.0) _biasSum = 0.5 + (0.5 * _biasSum * _biasSum);
                for (int l = 0; l < child.layerSizes[i-1]; l++) {
                    if (_weightSums[l] < 1.0) _weightSums[l] = 0.5 + (0.5 * _weightSums[l] * _weightSums[l]);
                }
            }

            // assign new values
            for (int l = 0; l < child.layerSizes[i-1]; l++) _weights[l] /= _weightSums[i];
            child.neurons[i][j].SetWeights(child.layerSizes[i-1], _weights);
            child.neurons[i][j].bias = _bias / _biasSum;
            delete [] _weights;
            delete [] _weightSums;
        }
    }
    return child;
}


NeuralNetwork NeuralNetwork::BinaryBreed(const int & _parentsCount, NeuralNetwork* _parents, float* _parentInfluences){
    if (_parentsCount == 0) return NeuralNetwork(); // if no parents return empty network
    if (_parentsCount == 1) return _parents[0]; // if only one parent -> child should be identical
    NeuralNetwork child(_parents[0]);

    float chanceSum = 0.0;
    for (int i = 0; i < _parentsCount; i++) chanceSum += _parentInfluences[i];

    for (int i = 1; i < child.layers; i++){
        for (int j = 0; j < child.layerSizes[i]; j++){
            // breed weights
            float* _weights = new float[child.layerSizes[i-1]];
            for (int l = 0; l < child.layerSizes[i-1]; l++) {
                // get random parent
                float rf = RandomFloat(0.0, chanceSum);
                int pId = 0;
                float chanceSum2 = _parentInfluences[pId];
                while (chanceSum2 < rf && pId < _parentsCount) {
                    pId++;
                    chanceSum2 += _parentInfluences[pId];
                }
                _weights[l] = _parents[pId].neurons[i][j].connections[l].weight; // assign random parent's weight to weight list for it's child
            }
            // assign new child's weights
            child.neurons[i][j].SetWeights(child.layerSizes[i-1], _weights);
            delete [] _weights;

            //breed bias
            // get random parent
            float rf = RandomFloat(0.0, chanceSum);
            int pId = 0;
            float chanceSum2 = _parentInfluences[pId];
            while (chanceSum2 < rf && pId < _parentsCount) {
                pId++;
                chanceSum2 += _parentInfluences[pId];
            }
            child.neurons[i][j].bias = _parents[pId].neurons[i][j].GetBias(); // assign random parent's bias to it's child
        }
    }
    return child;
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

float* NeuralNetwork::GetOutputs(){
    float* out = new float[layerSizes[layers-1]];
    for (int i = 0; i < layerSizes[layers-1]; i++){
        out[i] = neurons[layers-1][i].GetActivation();
    }
    return out;
}

float* NeuralNetwork::EvaluateGetOutputs(){
    EvaluateNetwork();
    return GetOutputs();
}

float* NeuralNetwork::EvaluateGetOutputs(const int & _count, float* _inputs){
    SetInputs(_count, _inputs);
    EvaluateNetwork();
    return GetOutputs();
}

int NeuralNetwork::GetDecision(){
    int out = 0;
    float _max = 0.0;
    for (int i = 0; i < layerSizes[layers-1]; i++){
        float _activation = neurons[layers-1][i].GetActivation();
        if (_activation > _max) {
            _max = _activation;
            out = i;
        }
    }
    return out;
}

int NeuralNetwork::EvaluateGetDecision(){
    EvaluateNetwork();
    return GetDecision();
}

int NeuralNetwork::EvaluateGetDecision(const int & _count, float* _inputs){
    SetInputs(_count, _inputs);
    EvaluateNetwork();
    return GetDecision();
}

int* NeuralNetwork::GetSortedDecisions(){
    // prepare data for sorting
    int siz = GetOutputsAmount();
    float* activations = GetOutputs();
    int* out = new int[siz]; // array of decision id's
    for (int i = 0; i < siz; i++){
        out[i] = i;
    }

    // bubble sort
    for (int j = 0; j < siz; j++){
        for (int i = 0; i < siz - j - 1; i++){
            if (activations[i] < activations[i+1]){
                float p = activations[i];
                activations[i] = activations[i+1];
                activations[i+1] = p;
                int d = out[i];
                out[i] = out[i+1];
                out[i+1] = d;
            }
        }
    }

    return out;
}

int* NeuralNetwork::EvaluateGetSortedDecisions(){
    EvaluateNetwork();
    return GetSortedDecisions();
}

int* NeuralNetwork::EvaluateGetSortedDecisions(const int & _count, float* _inputs){
    SetInputs(_count, _inputs);
    EvaluateNetwork();
    return GetSortedDecisions();
}

void NeuralNetwork::PrintInfo(const bool & _skipInputLayer, const bool & _neurons, const bool & _weights){
    printf("NeuralLayer Info:\nLayers: %d\n", layers);
    for (int i = (_skipInputLayer ? 1 : 0); i < layers; i++){
        printf("--Layer %d: Neurons: %d\n", i, layerSizes[i]);
        if (_neurons) {
            for (int j = 0; j < layerSizes[i]; j++){
                printf("----Neuron %d: ", j);
                neurons[i][j].PrintInfo(_weights);
            }
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