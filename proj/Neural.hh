#pragma once

float FastSigmoid(const float & x);
float RanodmFloat(const float & _min, const float & _max);
float* RandomList(const int & _size, const float & _min, const float & _max);

class Neuron{
    private:
    float activation = 0.0;
    float bias = 0.0;
    int connectionsCount = 0;

    struct WeightedConnection
    {
        float weight = 0.0;
        Neuron* neuron = nullptr;
    };
    WeightedConnection* connections = nullptr;

    public:
    Neuron();
    Neuron(const int & _connectionsCount, Neuron* _inputNeurons);
    Neuron(const Neuron & _neuron);
    void SetConnections(const int & _connectionsCount, Neuron* _inputNeurons);

    float GetActivation();
    void SetActivation(const float & _activation);
    float GetBias();
    void SetBias(const float & _bias);
    void SetWeights(float* _weights);
    float EvaluateActivation();

    void PrintInfo();

    ~Neuron();
};

// OBSOLETE, LACKS FULL IMPLEMENTATION
class NeuralLayer{
    Neuron *neurons;
    int neuronCount;
    public:
    NeuralLayer();
    NeuralLayer(const int & _neuronCount);
    void Initialise(const int & _neuronCount);
    void EvaluateLayer();
    void SetActivations(int* _activations);
    ~NeuralLayer();
};


class NeuralNetwork {
    private:
    Neuron ** neurons = nullptr;
    int layers = 0;
    int * layerSizes = nullptr;

    public:
    NeuralNetwork();
    NeuralNetwork(const int & _layers, int* _layerSizes);

    void RandomizeLayerWeights(const int & layerId);
    void RandomizeLayerBiases(const int & layerId);
    void RandomizeLayer(const int & layerId);
    void RandomizeNetworkWeights();
    void RandomizeNetworkBiases();
    void RandomizeNetwork();

    void SetInputs(const int & _count, int* _inputs);
    void EvaluateLayer(const int & layerId);
    void EvaluateNetwork();
    void EvaluateNetwork(const int & _count, int* _inputs);
    float* GetDecision();
    float* EvaluateGetDecision();
    float* EvaluateGetDecision(const int & _count, int* _inputs);

    void PrintInfo();

    ~NeuralNetwork();
};

// TESTS
void NeuronTest();
void NeuralNetworkTest();