#include "LearningAlgorithms.hh"
#include "UtilityFunctions.hh"
#include <stdio.h>
#include <random>
#include <time.h>


LearningAlgorithm::LearningAlgorithm():game(nullptr), generationSize(0), hiddenLayersCount(0), hiddenLayerSizes(nullptr){}

LearningAlgorithm::LearningAlgorithm(Game* _game, const int & _generationSize, const int & _hiddenLayersCount, int* _hiddenLayerSizes):game(_game), generationSize(_generationSize), hiddenLayersCount(_hiddenLayersCount), hiddenLayerSizes(nullptr){
    SetNetworkLayers(_hiddenLayersCount, _hiddenLayerSizes);
}

void LearningAlgorithm::SetGame(Game* _game){
    this->game = _game;
}
void LearningAlgorithm::SetNetworkLayers(const int & _hiddenLayersCount, int* _hiddenLayerSizes){
    this->hiddenLayersCount = _hiddenLayersCount;
    if (this->hiddenLayerSizes) delete [] this->hiddenLayerSizes;
    this->hiddenLayerSizes = new int[_hiddenLayersCount];
    for (int i = 0; i < _hiddenLayersCount; i++) this->hiddenLayerSizes[i] = _hiddenLayerSizes[i];
}
void LearningAlgorithm::SetGenerationSize(const int & _generationSize){
    this->generationSize = _generationSize;
}

NeuralNetwork* LearningAlgorithm::CreateGeneration(const float & _weightVariance, const float & _biasVariance) {
    srand(time(0)); // randomize seed for random number generator

    int gameInputs = game->GetInstanceInputs();
    int gameOutputs = game->GetInstanceOutputs();

    int _layers = hiddenLayersCount + 2;
    int* _layerSizes = new int[_layers];
    _layerSizes[0] = gameOutputs;
    for (int i = 1; i < _layers - 1; i++){
        _layerSizes[i] = hiddenLayerSizes[i-1];
    }
    _layerSizes[_layers-1] = gameInputs;

    NeuralNetwork* generation = new NeuralNetwork[generationSize];
    for (int i = 0; i < generationSize; i++){
        generation[i].SetupNetwork(_layers, _layerSizes);
        generation[i].RandomizeNetwork(_weightVariance, _biasVariance);
    }
    delete [] _layerSizes;
    printf("gen0 created\n");
    return generation;
}

NeuralNetwork LearningAlgorithm::TeachModel(const int & generationLimit, const int & gamesByOneModel) {
    int playersInGame = game->GetInstanceRequiredPlayersCount();
    NeuralNetwork* generation = CreateGeneration(5.0, 10.0);

    for (int i = 0; i < generationLimit; i++) { // for each generation
        printf("running gen%d\n", i);

        float* scores = new float[generationSize];
        for (int i = 0; i < generationSize; i++) scores[i] = 0.0;

        printf("playing ");
        for (int j = 0; j < generationSize; j++) { // for each model
            printf(".");
            Player** players = new Player*[playersInGame];
            AiModel* ap = new AiModel(generation[j]);
            players[0] = ap;
            if (playersInGame == 1) {
                for (int l = 0; l < gamesByOneModel; l++){ // play the amount of game specified in gamesByOneModel
                    game->SetPlayers(playersInGame, players);
                    game->Play(); // play the game
                    scores[j] += game->GetScore(0);
                }
            }

            else if (playersInGame == 2) {
                for (int l = 0; l < generationSize; l++) { // play against every model in generation
                    if (j != l) {
                        AiModel* op = new AiModel(generation[l]);
                        players[1] = op;
                        game->SetPlayers(playersInGame, players);
                        game->Play(); // play the game
                        scores[j] += game->GetScore(0);
                        scores[l] += game->GetScore(1);
                        delete op;
                    }
                }
            }
            delete ap;
            delete [] players;
        }
        printf("done\nsorting scores ... ");
        QuickSortStructuresByKey<NeuralNetwork, float>(generationSize, generation, scores, false);
        printf("done\nbest score: %f\nmedian score: %f\nworst score: %f\n", scores[0], scores[generationSize/2], scores[generationSize-1]);

        printf("normalizing scores ... ");
        float mscor = scores[0];
        for (int s = 0; s < generationSize; s++){
            scores[s] /= mscor;
            if (scores[s] < 0.0) scores[s] = 0.0;
            printf(".");
        }
        printf("done\nbreeding ");
        for (int i = generationSize / 2; i < generationSize; i++) {
            generation[i] = NeuralNetwork::Breed(generationSize / 2, generation, scores, 1.0);
            generation[i].VaryNetwork(0.1, 0.2);
            printf(".");
        }
        printf(" done\n");
        delete [] scores;
    }
    NeuralNetwork winner = generation[0];
    delete [] generation;
    return winner;
}

LearningAlgorithm::~LearningAlgorithm() {
    delete [] hiddenLayerSizes;
}