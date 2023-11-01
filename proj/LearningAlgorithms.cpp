#include "LearningAlgorithms.hh"
#include "UtilityFunctions.hh"
#include <stdio.h>
#include <thread>

#include <string>

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
    newGame.SetScoreParameters(0.0, 0.0, 0.1, 0.1, 1.0, 1.0);

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
    for (int i = 0; i < _repetitions / 2; i++){
        Player* playerList[2] = {&ai, _player};
        game.SetPlayers(2, playerList);
        game.Play(); // play the game
        score += game.GetScore(0);

        Player* playerList2[2] = {_player, &ai};
        game.SetPlayers(2, playerList2);
        game.Play(); // play the game
        score += game.GetScore(1);
    }
    return score / _repetitions;
}

LearningAlgorithm::~LearningAlgorithm() {
    delete [] hiddenLayerSizes;
}


/// ULTIMATE TIC TAC TOE

void UTTTLearning::RunGeneration(const int & siz, NeuralNetwork* generation, float * scores, const int & randomSuplementation, const bool & playSelf, const int & every, const int & start){
    int playersInGame = 2;
    UltimateTicTacToe newGame;
    newGame.SetScoreParameters(0.0, 0.01, 0.1, 0.1, 1.0, 1.0);

    for (int j = start; j < siz; j += every) { // for each model
        //printf("model %d in part %d\n", &generation[j], generation);
        printf(".");
        Player** players = new Player*[2];
        AiModel* ap = new AiModel(generation[j]);
        players[0] = ap;

        for (int l = start; playSelf && (l < siz); l += every) { // play against every model in generation
            if (j != l) {
                AiModel* op = new AiModel(generation[l]);
                players[1] = op;
                newGame.SetPlayers(playersInGame, players);
                newGame.Play(); // play the game
                scores[j] += newGame.GetScore(0);
                scores[l] += newGame.GetScore(1);
                delete op;
            }
        }

        for (int l = 0; l < randomSuplementation / 2; l++) { // play against random player
            RandomPlayer* rp = new RandomPlayer();
            players[1] = rp;
            newGame.SetPlayers(playersInGame, players);
            newGame.Play(); // play the game
            scores[j] += newGame.GetScore(0);

            players[0] = rp;
            players[1] = ap;
            newGame.SetPlayers(playersInGame, players);
            newGame.Play(); // play the game
            scores[j] += newGame.GetScore(1);

            players[0] = ap;
            delete rp;
        }

        delete ap;
        delete [] players;
    }
}

NeuralNetwork UTTTLearning::TeachModel(const int & generationSize, const int & subGenerations, const int & generationLimit) {
    int layerSizes[4] = {UltimateTicTacToe::GetOutputs(), 5, 5, UltimateTicTacToe::GetInputs()};
    NeuralNetwork* generation = LearningAlgorithm::CreateGeneration(generationSize, 4, layerSizes, 8.0, 0.2);

    for (int i = 0; i < generationLimit; i++) { // for each generation
        printf("running gen %d\n", i);

        float* scores = new float[generationSize];
        for (int i = 0; i < generationSize; i++) scores[i] = 0.0;

        int randomSuplementation = 50;
        bool playSelf = false;

        int sg = subGenerations;
        if (sg < 1) sg = 1;
        else if (sg > 8) sg = 8;

        printf("playing ");
        //std::thread subGenThread0(&LearningAlgorithm::RunSubGeneration, this, generationSize, generation, scores, gamesByOneModel);
        //subGenThread0.join();
        std::thread runThread1(UTTTLearning::RunGeneration, (sg > 0 ? generationSize : 0), generation, scores, randomSuplementation, playSelf, sg, 0);
        std::thread runThread2(UTTTLearning::RunGeneration, (sg > 1 ? generationSize : 0), generation, scores, randomSuplementation, playSelf, sg, 1);
        std::thread runThread3(UTTTLearning::RunGeneration, (sg > 2 ? generationSize : 0), generation, scores, randomSuplementation, playSelf, sg, 2);
        std::thread runThread4(UTTTLearning::RunGeneration, (sg > 3 ? generationSize : 0), generation, scores, randomSuplementation, playSelf, sg, 3);
        std::thread runThread5(UTTTLearning::RunGeneration, (sg > 4 ? generationSize : 0), generation, scores, randomSuplementation, playSelf, sg, 4);
        std::thread runThread6(UTTTLearning::RunGeneration, (sg > 5 ? generationSize : 0), generation, scores, randomSuplementation, playSelf, sg, 5);
        std::thread runThread7(UTTTLearning::RunGeneration, (sg > 6 ? generationSize : 0), generation, scores, randomSuplementation, playSelf, sg, 6);
        std::thread runThread8(UTTTLearning::RunGeneration, (sg > 7 ? generationSize : 0), generation, scores, randomSuplementation, playSelf, sg, 7);

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

        UltimateTicTacToe uttt;
        uttt.SetScoreParameters(0.0, 0.01, 0.1, 0.1, 1.0, 1.0);
        RandomPlayer rp;
        printf("winner against random: %f \n", LearningAlgorithm::TestAgainst(uttt, generation[0], &rp, 100));
        printf("median against random: %f \n", LearningAlgorithm::TestAgainst(uttt, generation[generationSize/2], &rp, 100));
        printf("loser against random: %f \n", LearningAlgorithm::TestAgainst(uttt, generation[generationSize-1], &rp, 100));
        Player np;
        printf("winner against naive: %f \n", LearningAlgorithm::TestAgainst(uttt, generation[0], &np, 2));
        printf("median against naive: %f \n", LearningAlgorithm::TestAgainst(uttt, generation[generationSize/2], &np, 2));
        printf("loser against naive: %f \n", LearningAlgorithm::TestAgainst(uttt, generation[generationSize-1], &np, 2));

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

        printf("breeding ");
        std::thread breedThread1(UTTTLearning::BreedGeneration, generationSize, generation, scores, 5, 4, 0);
        std::thread breedThread2(UTTTLearning::BreedGeneration, generationSize, generation, scores, 5, 4, 1);
        std::thread breedThread3(UTTTLearning::BreedGeneration, generationSize, generation, scores, 5, 4, 2);
        std::thread breedThread4(UTTTLearning::BreedGeneration, generationSize, generation, scores, 5, 4, 3);

        breedThread1.join();
        breedThread2.join();
        breedThread3.join();
        breedThread4.join();
        printf(" done\n");

        delete [] scores;
        std::string filename = "UTTTLearning/Evolutionary/GenW";
        filename.append(std::to_string(i));
        filename.append(".txt");
        NeuralNetwork::SaveNetwork(generation[0], filename);
    }
    NeuralNetwork winner = generation[0];
    delete [] generation;
    return winner;
}


void UTTTLearning::BreedGeneration(const int & siz, NeuralNetwork* generation, float * scores, const int & method, const int & every, const int & start){
    for (int i = (siz / 2) + start; i < siz; i += every) {
        if (method == 0) generation[i].RandomizeNetwork(1.0, 0.1);
        if (method == 1) generation[i] = NeuralNetwork::Breed(siz / 2, generation, scores, 0.0);
        if (method == 2) generation[i] = NeuralNetwork::Breed(siz / 2, generation, scores, 1.0);
        if (method == 3) generation[i] = NeuralNetwork::BinaryBreed(siz / 2, generation, scores);
        if (method == 4) generation[i] = NeuralNetwork::OneParentBreed(siz / 2, generation, scores);
        if (method == 5) generation[i] = generation[i - (siz / 2)];
        generation[i].VaryNetwork(0.1, 0.01);
        printf(".");
    }
}


NeuralNetwork UTTTLearning::BeatPredecessors(const int & generationLimit, const int & triesLimit){
    UltimateTicTacToe newGame;
    newGame.SetScoreParameters(0.00001, 0.001, 0.1, 0.1, 1.0, 1.0);
    Player** players = new Player*[2];

    NeuralNetwork* generations = new NeuralNetwork[generationLimit];
    int layerSizes[2] = {UltimateTicTacToe::GetOutputs(), UltimateTicTacToe::GetInputs()};
    generations[0].SetupNetwork(2, layerSizes);
    generations[0].RandomizeNetwork(0.1, 0.0);

    int tries = 0;
    int maxi = 1;
    printf("Running ");
    for (int i = 1; i < generationLimit; i++){
        if (i==maxi) {
            printf("gen %d ", i);
            tries = 0;
        }
        else i--;
        
        if(tries % (100 * i) == 0) printf(".");

        generations[i] = generations[i-1];
        //double factor = (((double)tries) / 100000.0) + 1.0;
        if (tries > triesLimit) {
            printf("tries limit exceded\n");
            return generations[i-1];
        }
        generations[i].VaryNetwork(0.5, 0.001);

        bool loss = false;
        AiModel* tested = new AiModel(generations[i]);
        for (int j = 0; j < i; j++)
        {
            AiModel* op = new AiModel(generations[j]);
            players[0] = tested;
            players[1] = op;
            newGame.SetPlayers(2, players);
            newGame.Play(); // play the game
            if (newGame.GetScore(0) <= newGame.GetScore(1)) {
                loss = true;
                tries++;
                delete op;
                break;
            }

            players[1] = tested;
            players[0] = op;
            newGame.SetPlayers(2, players);
            newGame.Play(); // play the game
            if (newGame.GetScore(0) >= newGame.GetScore(1)) {
                loss = true;
                tries++;
                delete op;
                break;
            }

            delete op;
        }
        delete tested;
        
        if (!loss){
            maxi++;
            std::string filename = "UTTTLearning/BeatPredec/Gen";
            filename.append(std::to_string(i));
            filename.append(".txt");
            NeuralNetwork::SaveNetwork(generations[i], filename);
            printf("\n");
        }
    }
    delete [] players;
    NeuralNetwork winner = generations[generationLimit-1];
    delete [] generations;
    return winner;
}