#pragma once

#include "Game.hh"
#include "Neural.hh"
#include "AiModel.hh"

class LearningAlgorithm {
    protected:
    Game* game;
    int* hiddenLayerSizes;
    int hiddenLayersCount;
    int generationSize;
    public:
    LearningAlgorithm();
    LearningAlgorithm(Game* _game, const int & _generationSize, const int & _hiddenLayersCount, int* _hiddenLayerSizes);

    void SetGame(Game* _game);
    void SetNetworkLayers(const int & _hiddenLayersCount, int* _hiddenLayerSizes);
    void SetGenerationSize(const int & _generationSize);

    virtual NeuralNetwork* CreateGeneration(const float & _weightVariance, const float & _biasVariance);
    virtual NeuralNetwork TeachModel(const int & generationLimit, const int & gamesByOneModel);

    ~LearningAlgorithm();
};