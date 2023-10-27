#pragma once

#include "player.hh"
#include "Neural.hh"

class AiModel : public Player{
    private:
    NeuralNetwork neuralNetwork;
    public:
    virtual int GetDecision();
};