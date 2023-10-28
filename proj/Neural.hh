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
    void CreateConnections(const int & _connectionsCount, Neuron* _inputNeurons);
    void CreateConnections(const int & _connectionsCount, Neuron* _inputNeurons, float* _weights);

    int GetConnectionsCount();
    float GetActivation();
    void SetActivation(const float & _activation);
    float GetBias();
    void SetBias(const float & _bias);
    float* GetWeights();
    void SetWeights(const int & _size, float* _weights);
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
    NeuralNetwork(const NeuralNetwork & _neuralNetwork);

    void RandomizeLayerWeights(const int & layerId);
    void RandomizeLayerBiases(const int & layerId);
    void RandomizeLayer(const int & layerId);
    void RandomizeNetworkWeights();
    void RandomizeNetworkBiases();
    void RandomizeNetwork();

    void SetInputs(const int & _count, float* _inputs);
    void EvaluateLayer(const int & layerId);
    void EvaluateNetwork();
    void EvaluateNetwork(const int & _count, float* _inputs);
    float* GetDecision();
    float* EvaluateGetDecision();
    float* EvaluateGetDecision(const int & _count, float* _inputs);

    void PrintInfo();

    ~NeuralNetwork();
};

// TESTS
void NeuronTest();
void NeuralNetworkTest();