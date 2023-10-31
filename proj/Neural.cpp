#include "Neural.hh"
#include "UtilityFunctions.hh"

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

Neuron::Neuron():activation(0), bias(0), connectionsCount(0), connections(nullptr){ }

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

Neuron::Neuron(const Neuron & other):activation(other.activation), bias(other.bias), connectionsCount(other.connectionsCount){
    this->connections = new WeightedConnection[this->connectionsCount];
    for (int i = 0; i < this->connectionsCount; i++) {
        this->connections[i].neuron = other.connections[i].neuron;
        this->connections[i].weight = other.connections[i].weight;
    }
}

Neuron& Neuron::operator=(const Neuron& other){
    if (this != &other){

        this->FreeExisting();

        this->connectionsCount = other.connectionsCount;
        this->bias = other.bias;
        // this->activation = other.activation; // activation needs to be evaluated -> no point of copying
        this->connections = new WeightedConnection[connectionsCount];
        for (int i = 0; i < this->connectionsCount; i++){
            this->connections[i].neuron = other.connections[i].neuron;
            this->connections[i].weight = other.connections[i].weight;
        }
    }
    return *this;
}

Neuron::Neuron(Neuron && other) : connections(nullptr), connectionsCount(0), activation(0.0), bias(0.0){
        this->connectionsCount = other.connectionsCount;
        this->bias = other.bias;
        // this->activation = other.activation; // activation needs to be evaluated -> no point of copying
        this->connections = other.connections;

        other.connectionsCount = 0;
        other.bias = 0.0;
        other.activation = 0.0;
        other.connections = nullptr;
}

Neuron& Neuron::operator=(Neuron&& other){
    if (this != &other){

        this->FreeExisting();

        this->connectionsCount = other.connectionsCount;
        this->bias = other.bias;
        // this->activation = other.activation; // activation needs to be evaluated anyways
        this->connections = other.connections;

        other.connectionsCount = 0;
        other.bias = 0.0;
        other.activation = 0.0;
        other.connections = nullptr;
    }
    return *this;
}

Neuron::~Neuron(){
    this->FreeExisting();
}

// free the existing resources
void Neuron::FreeExisting(){ // useful to avoid code duplication while writig desturctor and assignment operators
    if (this -> connections != nullptr) {
        delete [] connections;
    }
}

void Neuron::CreateNewConnections(const int & _connectionsCount, Neuron* _inputNeurons){
    this->FreeExisting();
    this->connectionsCount = _connectionsCount;
    this->connections = new WeightedConnection[_connectionsCount];
    for (int i = 0; i < _connectionsCount; i++)
    {
        //printf("connecting neuron %d: \n", &(_inputNeurons[i]));
        this->connections[i].neuron = &(_inputNeurons[i]);
    }
}

void Neuron::CreateNewConnections(const int & _connectionsCount, Neuron* _inputNeurons, float* _weights){
    this->FreeExisting();
    this->connectionsCount = _connectionsCount;
    this->connections = new WeightedConnection[_connectionsCount];
    for (int i = 0; i < _connectionsCount; i++)
    {
        this->connections[i].neuron = &_inputNeurons[i];
        this->connections[i].weight = _weights[i];
    }
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

void Neuron::SetBias(const float & _bias){
    this->bias = _bias;
}

float* Neuron::GetWeights(){
    float* out = new float[this->connectionsCount];
    for (int i = 0; i < connectionsCount; i++){
        out[i] = this->connections[i].weight;
    }
    return out;
}

void Neuron::SetWeights(const int & _size, float* _weights){
    for (int i = 0; i < connectionsCount && i <_size; i++){
        this->connections[i].weight = _weights[i];
    }
}

void Neuron::SetNeurons(const int & _size, Neuron* _neurons){
    for (int i = 0; i < connectionsCount && i <_size; i++){
        this->connections[i].neuron = &_neurons[i];
    }
}

void Neuron::SetConnections(const int & _size, Neuron* _neurons, float* _weights){
    for (int i = 0; i < connectionsCount && i <_size; i++){
        this->connections[i].neuron = &_neurons[i];
        this->connections[i].weight = _weights[i];
    }
}

void Neuron::VaryWeights(const int & _size, float* _weightVariances){
    for (int i = 0; i < connectionsCount && i <_size; i++){
        this->connections[i].weight += _weightVariances[i];
    }
}

float Neuron::EvaluateActivation(){
    float wSum = 0.0;
    for (int i = 0; i < this->connectionsCount; i++){
        float mul = this->connections[i].neuron->GetActivation() * this->connections[i].weight;
        //printf("acti of neuron %d: %f * conne weight: %f = %f\n", this->connections[i].neuron, this->connections[i].neuron->GetActivation(), this->connections[i].weight, mul);
        wSum += mul;
        //printf("wsum for now = %f\n", wSum);
    }
    wSum += bias;
    //printf("total wsum + bias = %f\n", wSum);
    wSum = FastSigmoid(wSum);
    //printf("after sigmoid: %f\n", wSum);
    this->activation = wSum;
    return this->activation;
}

void Neuron::PrintInfo(const bool & _weights, const bool & _neuronAdresses){
    printf("Activation: %f | Bias: %f | Connections: %d\n", activation, bias, connectionsCount);
    if (_weights || _neuronAdresses) {
        for(int i = 0; i < connectionsCount; i++){
            printf("--Connection %d: ", i);
            if (_weights) printf(" Wieght: %f ", connections[i].weight);
            if(_neuronAdresses) printf(" Neuron: %d", connections[i].neuron);
            printf("\n");
        }
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

NeuralNetwork::NeuralNetwork():layers(0), layerSizes(nullptr), neurons(nullptr){}

NeuralNetwork::NeuralNetwork(const int & _layers, int* _layerSizes):layers(_layers), layerSizes(new int[_layers]), neurons(new Neuron*[_layers]) {
    for (int i = 0; i <_layers; i++){
        this->layerSizes[i] = _layerSizes[i];
        this->neurons[i] = new Neuron[this->layerSizes[i]];
        for (int j = 0; i > 0 && j < _layerSizes[i]; j++){
            this->neurons[i][j].CreateNewConnections(_layerSizes[i-1], this->neurons[i-1]);
        }
    }
}

NeuralNetwork::NeuralNetwork(const NeuralNetwork & other):layers(other.layers), layerSizes(new int[other.layers]), neurons(new Neuron*[other.layers]) {
    for (int i = 0; i < this->layers; i++){
        this->layerSizes[i] = other.layerSizes[i];
        this->neurons[i] = new Neuron [this->layerSizes[i]];
        if (i==0) continue; // layer 0 neurons are always empty -> no point of copying
        for (int j = 0; j < this->layerSizes[i]; j++){
            this->neurons[i][j] = other.neurons[i][j];
            this->neurons[i][j].SetNeurons(this->layerSizes[i-1], this->neurons[i-1]);
        }
    }
}

NeuralNetwork& NeuralNetwork::operator=(const NeuralNetwork& other) {
    if (this != &other){

        this->FreeExisting();

        this->layers = other.layers;
        this->layerSizes = new int [this->layers];
        this->neurons = new Neuron* [this->layers];

        for (int i = 0; i < this->layers; i++) {
            this->layerSizes[i] = other.layerSizes[i];
            this->neurons[i] = new Neuron [this->layerSizes[i]];
            if (i==0) continue;
            for (int j = 0; j < this->layerSizes[i]; j++){
                this->neurons[i][j] = other.neurons[i][j];
                this->neurons[i][j].SetNeurons(this->layerSizes[i-1], this->neurons[i-1]);
            }
        }
    }
    return *this;
}

NeuralNetwork::NeuralNetwork(NeuralNetwork && other) : neurons(nullptr), layers(0), layerSizes(nullptr){
    this->layers = other.layers;
    this->layerSizes = other.layerSizes;
    this->neurons = other.neurons;

    other.layers = 0;
    other.layerSizes = nullptr;
    other.neurons = nullptr;
}

NeuralNetwork& NeuralNetwork::operator=(NeuralNetwork&& other) {
    if (this != &other){

        this->FreeExisting();

        this->layers = other.layers;
        this->layerSizes = other.layerSizes;
        this->neurons = other.neurons;
    
        other.layers = 0;
        other.layerSizes = nullptr;
        other.neurons = nullptr;
    }
    return *this;
}

NeuralNetwork::~NeuralNetwork(){
    this->FreeExisting();
}

// free the existing resources
void NeuralNetwork::FreeExisting(){ // useful to avoid code duplication while writig desturctor and assignment operators
    if (this->neurons != nullptr) {
        for (int i = 0; i < this->layers; i++){
            if (this->neurons[i] != nullptr) delete [] this->neurons[i];
        }
        delete [] this->neurons;
    }
    if (this->layerSizes != nullptr) delete [] this->layerSizes;
}

void NeuralNetwork::SetupNetwork(const int & _layers, int* _layerSizes){
    this->FreeExisting();
    this->layers = _layers;
    this->neurons = new Neuron*[_layers];
    this->layerSizes = new int[_layers];
    for (int i = 0; i < this->layers; i++){
        this->layerSizes[i] = _layerSizes[i];
        this->neurons[i] = new Neuron[this->layerSizes[i]];
        for (int j = 0; i > 0 && j < this->layerSizes[i]; j++){
            this->neurons[i][j].CreateNewConnections(_layerSizes[i-1], this->neurons[i-1]);
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
        float* _weights = RandomList(layerSizes[layerId-1], -_weightVariance, _weightVariance);
        neurons[layerId][i].SetWeights(layerSizes[layerId-1], _weights);
        delete [] _weights;
        float _bias = RandomFloat(-_biasVariance, _biasVariance);
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
    NeuralNetwork child = _parents[0]; // child should have the same structure as parents
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

                delete [] _weights2;

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

    NeuralNetwork child = _parents[0]; // child should have the same structure as parents

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
        //printf("neuron %d from layer: %d - ", i, layerId);
        //neurons[layerId][i].PrintInfo(false, false);
    }
}

void NeuralNetwork::EvaluateNetwork(){
    /*printf("evaluating inputs: \n");
    for (int i = 0; i < layerSizes[0]; i++){
        printf("neuron %d from layer: %d - ", i, 0);
        neurons[0][i].PrintInfo(false, false);
    }*/
    for (int i = 1; i < layers; i++){
        EvaluateLayer(i);
    }
    //PrintInfo(false, true, false);
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

    QuickSortStructuresByKey<int, float>(siz, out, activations, false);

    delete [] activations;
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
                neurons[i][j].PrintInfo(_weights, _weights);
            }
        }
    }
}