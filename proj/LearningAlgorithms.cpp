#include "LearningAlgorithms.hh"
#include "UtilityFunctions.hh"
#include <stdio.h>
#include <thread>


LearningAlgorithm::LearningAlgorithm():game(nullptr), generationSize(0), hiddenLayersCount(0), hiddenLayerSizes(nullptr){}

LearningAlgorithm::LearningAlgorithm(Game* _game):game(_game), generationSize(0), generationsCount(0), hiddenLayersCount(0), hiddenLayerSizes(nullptr){ }

void LearningAlgorithm::SetGame(Game* _game){
    this->game = _game;
}
void LearningAlgorithm::SetNetworkNetworkParameters(const int & _hiddenLayersCount, int* _hiddenLayerSizes){
    this->hiddenLayersCount = _hiddenLayersCount;
    if (this->hiddenLayerSizes) delete [] this->hiddenLayerSizes;
    this->hiddenLayerSizes = new int[_hiddenLayersCount];
    for (int i = 0; i < _hiddenLayersCount; i++) this->hiddenLayerSizes[i] = _hiddenLayerSizes[i];
}
void LearningAlgorithm::SetGenerationParameters(const int & _generationSize, const int & _amountOfGenerations){
    this->generationSize = _generationSize;
    this->generationsCount = _amountOfGenerations;
}

NeuralNetwork* LearningAlgorithm::CreateGeneration(const int & generationSize, const int & layersCount, int* layerSizes, const float & _weightVariance, const float & _biasVariance) {
    NeuralNetwork* generation = new NeuralNetwork[generationSize];
    for (int i = 0; i < generationSize; i++){
        generation[i].SetupNetwork(layersCount, layerSizes);
        generation[i].RandomizeNetwork(_weightVariance, _biasVariance);
    }
    printf("gen0 created\n");
    return generation;
}


void LearningAlgorithm::RunSubGeneration(const int & siz, NeuralNetwork* generation, float * privScores, const int & gamesByOneModel){
    int playersInGame = game->GetInstanceRequiredPlayersCount();
    UltimateTicTacToe newGame;
    newGame.SetScoreParameters(0.0, 0.0, 1.0, 1.0);

    for (int j = 0; j < siz; j++) { // for each model
        //printf("model %d in part %d\n", &generation[j], generation);
        printf(".");
        Player** players = new Player*[playersInGame];
        AiModel* ap = new AiModel(generation[j]);
        players[0] = ap;
        if (playersInGame == 1) {
            for (int l = 0; l < gamesByOneModel; l++){ // play the amount of game specified in gamesByOneModel
                newGame.SetPlayers(playersInGame, players);
                newGame.Play(); // play the game
                privScores[j] += newGame.GetScore(0);
            }
        }

        else if (playersInGame == 2) {
            for (int l = 0; l < siz; l++) { // play against every model in generation
                if (j != l) {
                    AiModel* op = new AiModel(generation[l]);
                    players[1] = op;
                    newGame.SetPlayers(playersInGame, players);
                    newGame.Play(); // play the game
                    privScores[j] += newGame.GetScore(0);
                    privScores[l] += newGame.GetScore(1);
                    delete op;
                }
            }
            for (int l = 0; l < gamesByOneModel; l++) { // play against random player
                RandomPlayer* rp = new RandomPlayer();
                players[1] = rp;
                newGame.SetPlayers(playersInGame, players);
                newGame.Play(); // play the game
                privScores[j] += newGame.GetScore(0);
                //printf("%f\n", newGame->GetScore(0));
                delete rp;
            }
        }
        delete ap;
        delete [] players;
    }
}

NeuralNetwork LearningAlgorithm::TeachModel(const int & generationLimit, const int & gamesByOneModel) {
    int gameInputs = game->GetInstanceInputs();
    int gameOutputs = game->GetInstanceOutputs();
    int layerSizes[4] = {gameOutputs, 16, 16, gameInputs};
    NeuralNetwork* generation = LearningAlgorithm::CreateGeneration(generationSize, 4, layerSizes, 5.0, 10.0);

    for (int i = 0; i < generationLimit; i++) { // for each generation
        printf("running gen %d\n", i);

        float* scores = new float[generationSize];
        for (int i = 0; i < generationSize; i++) scores[i] = 0.0;

        printf("playing ");
        
        int genSiz = generationSize / 8;
        //std::thread subGenThread0(&LearningAlgorithm::RunSubGeneration, this, generationSize, generation, scores, gamesByOneModel);
        //subGenThread0.join();
        std::thread subGenThread1(&LearningAlgorithm::RunSubGeneration, this, genSiz, &generation[0*genSiz], &scores[0*genSiz], gamesByOneModel);
        std::thread subGenThread2(&LearningAlgorithm::RunSubGeneration, this, genSiz, &generation[1*genSiz], &scores[1*genSiz], gamesByOneModel);
        std::thread subGenThread3(&LearningAlgorithm::RunSubGeneration, this, genSiz, &generation[2*genSiz], &scores[2*genSiz], gamesByOneModel);
        std::thread subGenThread4(&LearningAlgorithm::RunSubGeneration, this, genSiz, &generation[3*genSiz], &scores[3*genSiz], gamesByOneModel);
        std::thread subGenThread5(&LearningAlgorithm::RunSubGeneration, this, genSiz, &generation[4*genSiz], &scores[4*genSiz], gamesByOneModel);
        std::thread subGenThread6(&LearningAlgorithm::RunSubGeneration, this, genSiz, &generation[5*genSiz], &scores[5*genSiz], gamesByOneModel);
        std::thread subGenThread7(&LearningAlgorithm::RunSubGeneration, this, genSiz, &generation[6*genSiz], &scores[6*genSiz], gamesByOneModel);
        std::thread subGenThread8(&LearningAlgorithm::RunSubGeneration, this, genSiz, &generation[7*genSiz], &scores[7*genSiz], gamesByOneModel);

        subGenThread1.join();
        subGenThread2.join();
        subGenThread3.join();
        subGenThread4.join();
        subGenThread5.join();
        subGenThread6.join();
        subGenThread7.join();
        subGenThread8.join();
        
        printf("done\nsorting scores ... ");
        QuickSortStructuresByKey<NeuralNetwork, float>(generationSize, generation, scores, false);
        printf("done\nbest score: %f\nmedian score: %f\nworst score: %f\n", scores[0], scores[generationSize/2], scores[generationSize-1]);

        printf("normalizing scores ");
        float maxscor = scores[0];
        if (maxscor <= 0.0){
            float minscor = scores[generationSize-1];
            for (int s = 0; s < generationSize; s++) scores[s] += minscor;
            maxscor = scores[0];
        }
        for (int s = 0; s < generationSize; s++){
            scores[s] /= maxscor;
            if (scores[s] < 0.0) scores[s] = 0.0;
            printf(".");
        }
        printf("done\nbreeding ");
        for (int i = generationSize / 2; i < generationSize; i++) {
            //generation[i] = NeuralNetwork::Breed(generationSize / 2, generation, scores, 1.0);
            generation[i] = generation[i - (generationSize / 2)];
            generation[i].VaryNetwork(0.1, 0.4);
            printf(".");
        }
        printf(" done\ntest winner against naive player: ");
        RandomPlayer rp;
        printf("average score: %f \n", TestAgainst(*game, generation[0], &rp, 50));
        delete [] scores;
    }
    NeuralNetwork winner = generation[0];
    delete [] generation;
    return winner;
}

float LearningAlgorithm::TestAgainst(Game & game, const NeuralNetwork & nn, Player * _player, const int & _repetitions){
    float score = 0.0;
    AiModel ai(nn);
    for (int i = 0; i < _repetitions; i++){
        Player* playerList[2] = {&ai, _player};
        game.SetPlayers(2, playerList);
        game.Play(); // play the game
        score += game.GetScore(0);
    }
    return score / _repetitions;
}

LearningAlgorithm::~LearningAlgorithm() {
    delete [] hiddenLayerSizes;
}


/// ULTIMATE TIC TAC TOE

void UTTTLearning::RunGeneration(const int & siz, NeuralNetwork* generation, float * scores, const int & randomSuplementation, const int & every, const int & start){
    int playersInGame = 2;
    UltimateTicTacToe newGame;
    newGame.SetScoreParameters(0.01, 0.0, 0.0, 0.0);

    for (int j = start; j < siz; j += every) { // for each model
        //printf("model %d in part %d\n", &generation[j], generation);
        printf(".");
        Player** players = new Player*[2];
        AiModel* ap = new AiModel(generation[j]);
        players[0] = ap;

        /*for (int l = start; l < siz; l += every) { // play against every model in generation
            if (j != l) {
                AiModel* op = new AiModel(generation[l]);
                players[1] = op;
                newGame.SetPlayers(playersInGame, players);
                newGame.Play(); // play the game
                scores[j] += newGame.GetScore(0);
                scores[l] += newGame.GetScore(1);
                delete op;
            }
        }*/

        for (int l = 0; l < randomSuplementation; l++) { // play against random player
            RandomPlayer* rp = new RandomPlayer();
            players[1] = rp;
            newGame.SetPlayers(playersInGame, players);
            newGame.Play(); // play the game
            scores[j] += newGame.GetScore(0);
            delete rp;
        }

        delete ap;
        delete [] players;
    }
}

NeuralNetwork UTTTLearning::TeachModel(const int & generationSize, const int & subGenerations, const int & generationLimit) {
    int layerSizes[4] = {UltimateTicTacToe::GetOutputs(), 16, 16, UltimateTicTacToe::GetInputs()};
    NeuralNetwork* generation = LearningAlgorithm::CreateGeneration(generationSize, 4, layerSizes, 5.0, 10.0);

    for (int i = 0; i < generationLimit; i++) { // for each generation
        printf("running gen %d\n", i);

        float* scores = new float[generationSize];
        for (int i = 0; i < generationSize; i++) scores[i] = 0.0;

        int randomSuplementation = 50;

        int sg = subGenerations;
        if (sg < 1) sg = 1;
        else if (sg > 8) sg = 8;

        printf("playing ");
        //std::thread subGenThread0(&LearningAlgorithm::RunSubGeneration, this, generationSize, generation, scores, gamesByOneModel);
        //subGenThread0.join();
        std::thread runThread1(UTTTLearning::RunGeneration, (sg > 0 ? generationSize : 0), generation, scores, randomSuplementation, sg, 0);
        std::thread runThread2(UTTTLearning::RunGeneration, (sg > 1 ? generationSize : 0), generation, scores, randomSuplementation, sg, 1);
        std::thread runThread3(UTTTLearning::RunGeneration, (sg > 2 ? generationSize : 0), generation, scores, randomSuplementation, sg, 2);
        std::thread runThread4(UTTTLearning::RunGeneration, (sg > 3 ? generationSize : 0), generation, scores, randomSuplementation, sg, 3);
        std::thread runThread5(UTTTLearning::RunGeneration, (sg > 4 ? generationSize : 0), generation, scores, randomSuplementation, sg, 4);
        std::thread runThread6(UTTTLearning::RunGeneration, (sg > 5 ? generationSize : 0), generation, scores, randomSuplementation, sg, 5);
        std::thread runThread7(UTTTLearning::RunGeneration, (sg > 6 ? generationSize : 0), generation, scores, randomSuplementation, sg, 6);
        std::thread runThread8(UTTTLearning::RunGeneration, (sg > 7 ? generationSize : 0), generation, scores, randomSuplementation, sg, 7);

        runThread1.join();
        runThread2.join();
        runThread3.join();
        runThread4.join();
        runThread5.join();
        runThread6.join();
        runThread7.join();
        runThread8.join();
        printf("done\n");
        
        printf("sorting scores ... ");
        QuickSortStructuresByKey<NeuralNetwork, float>(generationSize, generation, scores, false);
        printf("done\nbest score: %f\nmedian score: %f\nworst score: %f\n", scores[0], scores[generationSize/2], scores[generationSize-1]);

        printf("normalizing scores ");
        float maxscor = scores[0];
        if (maxscor <= 0.0){
            float minscor = scores[generationSize-1];
            for (int s = 0; s < generationSize; s++) scores[s] += minscor;
            maxscor = scores[0];
        }
        for (int s = 0; s < generationSize; s++){
            scores[s] /= maxscor;
            if (scores[s] < 0.0) scores[s] = 0.0;
            printf(".");
        }
        printf("done\n");

        UltimateTicTacToe uttt;
        uttt.SetScoreParameters(0.01, 0.0, 0.0, 0.0);
        RandomPlayer rp;
        printf("winner against random: %f \n", LearningAlgorithm::TestAgainst(uttt, generation[0], &rp, 50));
        printf("loser against random: %f \n", LearningAlgorithm::TestAgainst(uttt, generation[generationSize-1], &rp, 50));

        printf("breeding ");
        std::thread breedThread1(UTTTLearning::BreedGeneration, generationSize, generation, scores, 4, 4, 0);
        std::thread breedThread2(UTTTLearning::BreedGeneration, generationSize, generation, scores, 1, 4, 1);
        std::thread breedThread3(UTTTLearning::BreedGeneration, generationSize, generation, scores, 4, 4, 2);
        std::thread breedThread4(UTTTLearning::BreedGeneration, generationSize, generation, scores, 0, 4, 3);

        breedThread1.join();
        breedThread2.join();
        breedThread3.join();
        breedThread4.join();
        printf(" done\n");

        delete [] scores;
    }
    NeuralNetwork winner = generation[0];
    delete [] generation;
    return winner;
}


void UTTTLearning::BreedGeneration(const int & siz, NeuralNetwork* generation, float * scores, const int & method, const int & every, const int & start){
    for (int i = (siz / 2) + start; i < siz; i += every) {
        if (method == 0) generation[i].RandomizeNetwork(5.0, 10.0);
        if (method == 1) generation[i] = NeuralNetwork::Breed(siz / 2, generation, scores, 0.0);
        if (method == 2) generation[i] = NeuralNetwork::Breed(siz / 2, generation, scores, 1.0);
        if (method == 3) generation[i] = NeuralNetwork::BinaryBreed(siz / 2, generation, scores);
        if (method == 4) generation[i] = NeuralNetwork::OneParentBreed(siz / 2, generation, scores);
        generation[i].VaryNetwork(0.2, 0.4);
        printf(".");
    }
}