class Player
{
    public:
    virtual int GetDecision(const int & _inputSize, float * inputs, const int & requestNo);
    virtual int GetDecision(const int & _inputSize, int * inputs, const int & requestNo);
};