#include "player.hh"
#include <stdio.h>
#include "random"

// PLAYER INTERFACE

float* Player::ConvertIntToFloat(const int & _size, int * _values, const bool & normalize){
    float _max = 1.0;
    float * floatValues = new float [_size];
    for (int i = 0; i < _size; i++){
        floatValues[i] = _values[i];
        if (normalize && floatValues[i] > _max) _max = floatValues[i];
    }
    if (normalize) {
        for (int i = 0; i < _size; i++) {
            floatValues[i] /= _max;
        }
    }
    return floatValues;
}

int Player::GetDecision(const int & _inputSize, float * inputs, const int & requestNo){
    return requestNo;
}


// HUMAN PLAYER

HumanPlayer::HumanPlayer(){
    this->requireDisplay = true;
}

int HumanPlayer::GetDecision(const int & _inputSize, float * inputs, const int & requestNo){
    int out = 0;
    scanf("%d", &out);
    return out;
}


// RANDOM PLAYER

RandomPlayer::RandomPlayer(){
    this->requireDisplay = false;
}

int RandomPlayer::GetDecision(const int & _inputSize, float * _inputs, const int & requestNo){
    return rand() % _inputSize;
}