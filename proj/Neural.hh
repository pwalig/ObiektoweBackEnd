#pragma once
#include <string>

float FastSigmoid(const float & x);
float ReLU(const float & x);
float RandomFloat(const float & _min, const float & _max);
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

    void FreeExisting();

    public:
    Neuron();
    Neuron(const int & _connectionsCount, Neuron* _inputNeurons);
    Neuron(const int & _connectionsCount, Neuron* _inputNeurons, float* _weights);
    Neuron(const Neuron & _neuron);
    Neuron(Neuron && _neuron);

    Neuron& operator=(const Neuron& other);
    Neuron& operator=(Neuron&& other);

    void CreateNewConnections(const int & _connectionsCount, Neuron* _inputNeurons);
    void CreateNewConnections(const int & _connectionsCount, Neuron* _inputNeurons, float* _weights);

    int GetConnectionsCount();
    float GetActivation();
    void SetActivation(const float & _activation);
    float GetBias();
    void SetBias(const float & _bias);
    float* GetWeights();
    void SetWeights(const int & _size, float* _weights);
    void SetNeurons(const int & _size, Neuron* _neurons);
    void SetConnections(const int & _size, Neuron* _neurons, float* _weights);

    void VaryWeights(const int & _size, float* _weightVariances);

    float EvaluateActivation();

    void PrintInfo(const bool & _weights, const bool & _neuronAdresses);

    ~Neuron();

    friend class NeuralNetwork;
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
    int layers = 0;
    int * layerSizes = nullptr;
    Neuron ** neurons = nullptr;

    void FreeExisting();

    public:
    NeuralNetwork();
    NeuralNetwork(const int & _layers, int* _layerSizes);
    NeuralNetwork(const NeuralNetwork & other);
    NeuralNetwork(NeuralNetwork && other);

    NeuralNetwork& operator=(const NeuralNetwork& other);
    NeuralNetwork& operator=(NeuralNetwork&& other);

    void SetupNetwork(const int & _layers, int* _layerSizes);

    int GetLayersAmount();
    int* GetLayerSizes();
    int GetInputsAmount();
    int GetOutputsAmount();

    void RandomizeLayerWeights(const int & layerId, const float & _weightVariance);
    void RandomizeLayerBiases(const int & layerId, const float & _biasVariance);
    void RandomizeLayer(const int & layerId, const float & _weightVariance, const float & _biasVariance);
    void RandomizeNetworkWeights(const float & _weightVariance);
    void RandomizeNetworkBiases(const float & _biasVariance);
    void RandomizeNetwork(const float & _weightVariance, const float & _biasVariance);

    void VaryLayerWeights(const int & layerId, const float & _weightVariance);
    void VaryLayerBiases(const int & layerId, const float & _biasVariance);
    void VaryLayer(const int & layerId, const float & _weightVariance, const float & _biasVariance);
    void VaryNetworkWeights(const float & _weightVariance);
    void VaryNetworkBiases(const float & _biasVariance);
    void VaryNetwork(const float & _weightVariance, const float & _biasVariance);

    static NeuralNetwork Breed(const int & _parentsCount, NeuralNetwork* _parents, float* _parentInfluences, const float & variance);
    static NeuralNetwork BinaryBreed(const int & _parentsCount, NeuralNetwork* _parents, float* _parentInfluences);
    static NeuralNetwork OneParentBreed(const int & _parentsCount, NeuralNetwork* _parents, float* _parentInfluences);

    void SetInputs(const int & _count, float* _inputs);
    void EvaluateLayer(const int & layerId);
    void EvaluateNetwork();
    void EvaluateNetwork(const int & _count, float* _inputs);
    float* GetOutputs();
    float* EvaluateGetOutputs();
    float* EvaluateGetOutputs(const int & _count, float* _inputs);
    int GetDecision();
    int EvaluateGetDecision();
    int EvaluateGetDecision(const int & _count, float* _inputs);
    int* GetSortedDecisions();
    int* EvaluateGetSortedDecisions();
    int* EvaluateGetSortedDecisions(const int & _count, float* _inputs);

    void PrintInfo(const bool & _skipInputLayer, const bool & _neurons, const bool & _weights);

    void SaveNetwork(const std::string & filename);
    static void SaveNetwork(const NeuralNetwork & nn, const std::string & filename);
    static NeuralNetwork ReadNetwork(const std::string & filename);

    ~NeuralNetwork();
};