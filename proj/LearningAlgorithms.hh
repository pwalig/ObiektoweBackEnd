#pragma once

#include "Game.hh"
#include "UltimateTicTacToe.hh"
#include "Neural.hh"
#include "AiModel.hh"

class LearningAlgorithm {
    protected:
    Game* game;

    int* hiddenLayerSizes;
    int hiddenLayersCount;

    int generationSize;
    int generationsCount;

    void AgainstEveryone(const int & siz, NeuralNetwork* generation, const int & skip, float & privScore);
    void AgainstRandom(const NeuralNetwork testedOne, const int & _repetitions, float & privScore);
    virtual void RunSubGeneration(const int & siz, NeuralNetwork* generation, float * privScores, const int & gamesByOneModel);
    
    public:
    LearningAlgorithm();
    LearningAlgorithm(Game* _game);

    void SetGame(Game* _game);
    void SetNetworkNetworkParameters(const int & _hiddenLayersCount, int* _hiddenLayerSizes);
    void SetGenerationParameters(const int & _generationSize, const int & _amountOfGenerations);

    static NeuralNetwork* CreateGeneration(const int & generationSize, const int & hiddenLayersCount, int* hiddenLayerSizes, const float & _weightVariance, const float & _biasVariance);
    virtual NeuralNetwork TeachModel(const int & generationLimit, const int & gamesByOneModel);

    static float TestAgainst(Game & game, const NeuralNetwork & nn, Player * _player, const int & _repetitions);

    ~LearningAlgorithm();
};


class UTTTLearning {
    private:
    static void RunGeneration(const int & siz, NeuralNetwork* generation, float * scores, const int & gamesByOneModel, const int & every, const int & start);
    static void BreedGeneration(const int & siz, NeuralNetwork* generation, float * scores, const int & method, const int & every, const int & start);

    public:
    static NeuralNetwork TeachModel(const int & generationSize, const int & subGenerations, const int & generationLimit);
};

