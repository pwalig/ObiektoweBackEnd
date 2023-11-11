#pragma once
#include <cmath> // sqrt()


//----------MathVector Declaration----------

template <typename T, size_t U>
class MathVector;


//----------Function Declarations----------

template <typename T>
MathVector<T, 2> CrossProduct2(const MathVector<T, 2> & mv);

template <typename T>
MathVector<T, 3> CrossProduct3(const MathVector<T, 3> & mv1, const MathVector<T, 3> & mv2);

template <typename T, size_t U>
T DotProduct(const MathVector<T, U> & mv1, const MathVector<T, U> & mv2);


//----------Math Vector Declarations----------

template <typename T, size_t U>
class MathVector {
    public:
    T* fields;

    MathVector();
    MathVector(int init);
    MathVector(const MathVector & other);
    MathVector(MathVector && other);
    ~MathVector();

    MathVector& operator=(const MathVector& other);
    MathVector& operator=(MathVector&& other);
    MathVector& operator+=(const MathVector<T, U> mv);
    MathVector& operator-=(const MathVector<T, U> mv);
    template <typename V> MathVector& operator*=(const V & va);
    template <typename V> MathVector& operator/=(const V & va);
    MathVector operator+(const MathVector<T, U>& mv);
    MathVector operator-(const MathVector<T, U>& mv);
    MathVector operator-();
    template <typename V> MathVector operator*(const V & va);
    template <typename V> MathVector operator/(const V & va);
    bool operator==(const MathVector<T, U>& mv);
    bool operator!=(const MathVector<T, U>& mv);

    T GetMagnitude();
};


//----------Constructor Definitions----------

template <typename T, size_t U>
MathVector<T, U>::MathVector(){
    fields = new T[U];
}

template <typename T, size_t U>
MathVector<T, U>::MathVector(int init){
    fields = new T[U];
    for (int i = 0; i < U; i++)
        fields[i] = init;
}

template <typename T, size_t U>
MathVector<T, U>::MathVector(const MathVector & other) : fields(nullptr){
    fields = new T[U];
    for (int i = 0; i < U; i++)
        fields[i] = other.fields[i];
}

template <typename T, size_t U>
MathVector<T, U>::MathVector(MathVector && other) : fields(other.fields){
    other.fields = nullptr;
}

template <typename T, size_t U>
MathVector<T, U>::~MathVector(){
    delete fields;
}


//----------Function Definitions----------

template <typename T>
MathVector<T, 2> CrossProduct2(const MathVector<T, 2> & mv){
    MathVector<T, 2> r;
    r.fields[0] = -mv.fields[1];
    r.fields[1] = mv.fields[0];
    return r;
}
template <typename T>
MathVector<T, 3> CrossProduct3(const MathVector<T, 3> & mv1, const MathVector<T, 3> & mv2){
    MathVector<T, 3> r;
    r.fields[0] = (mv1.fields[2] * mv2.fields[3]) - (mv1.fields[3] * mv2.fields[2]);
    r.fields[1] = (mv1.fields[3] * mv2.fields[1]) - (mv1.fields[1] * mv2.fields[3]);
    r.fields[2] = (mv1.fields[1] * mv2.fields[2]) - (mv1.fields[2] * mv2.fields[1]);
    return r;
}
template <typename T, size_t U>
T DotProduct(const MathVector<T, U> & mv1, const MathVector<T, U> & mv2){
    T r = 0;
    for (int i = 0; i<U; i++){
        r += mv1.fields[i] * mv2.fields[i];
    }
    return r;
}


//----------Method Definitions----------

template <typename T, size_t U>
T MathVector<T, U>::GetMagnitude(){
    T sum = 0;
    for (int i = 0; i<U; i++){
        sum += (this->fields[i] * this->fields[i]);
    }
    return sqrt(sum);
}


//----------Operator Definitions----------

template <typename T, size_t U>
MathVector<T, U>& MathVector<T, U>::operator=(const MathVector& other){
    if (this != other){
        for (int i = 0; i < U; i++){
            this->fields[i] = other.fields[i];
        }
    }
    return *this;
}
template <typename T, size_t U>
MathVector<T, U>& MathVector<T, U>::operator=(MathVector&& other){
    if (this != other){
        delete fields;
        this->fields = other.fields;
        other.fields = nullptr;
    }
    return *this;
}

template <typename T, size_t U>
MathVector<T, U>& MathVector<T, U>::operator+=(const MathVector<T, U> mv){
    for (int i = 0; i < U; i++) {
        this->fields[i] += mv.fields[i];
    }
    return *this;
}
template <typename T, size_t U>
MathVector<T, U>& MathVector<T, U>::operator-=(const MathVector<T, U> mv){
    for (int i = 0; i < U; i++) {
        this->fields[i] -= mv.fields[i];
    }
    return *this;
}
template <typename T, size_t U> template <typename V>
MathVector<T, U>& MathVector<T, U>::operator*=(const V & va){
    for (int i = 0; i < U; i++) {
        this->fields[i] *= va;
    }
    return *this;
}
template <typename T, size_t U> template <typename V> 
MathVector<T, U>& MathVector<T, U>::operator/=(const V & va){
    for (int i = 0; i < U; i++) {
        this->fields[i] /= va;
    }
    return *this;
}
template <typename T, size_t U>
MathVector<T, U> MathVector<T, U>::operator+(const MathVector<T, U>& mv) {
    MathVector<T,U> r;
    for (int i = 0; i<U; i++) {
        r.fields[i] = this->fields[i] + mv.fields[i];
    }
    return r;
}
template <typename T, size_t U>
MathVector<T, U> MathVector<T, U>::operator-(const MathVector<T, U>& mv){
    MathVector<T,U> r;
    for (int i = 0; i<U; i++) {
        r.fields[i] = this->fields[i] - mv.fields[i];
    }
    return r;
}
template <typename T, size_t U>
MathVector<T, U> MathVector<T, U>::operator-(){
    MathVector<T,U> r;
    for (int i = 0; i<U; i++) {
        r.fields[i] = -this->fields[i];
    }
    return r;
}
template <typename T, size_t U> template <typename V>
MathVector<T, U> MathVector<T, U>::operator*(const V & va){
    MathVector<T,U> r;
    for (int i = 0; i<U; i++) {
        r.fields[i] = this->fields[i] * va;
    }
    return r;
}
template <typename T, size_t U> template <typename V>
MathVector<T, U> MathVector<T, U>::operator/(const V & va){
    MathVector<T,U> r;
    for (int i = 0; i<U; i++) {
        r.fields[i] = this->fields[i] / va;
    }
    return r;
}
template <typename T, size_t U>
bool MathVector<T, U>::operator==(const MathVector<T, U>& mv){
    for (int i = 0; i<U; i++){
        if (this->fields[i] != mv.fields[i]) return false;
    }
    return true;
}
template <typename T, size_t U>
bool MathVector<T, U>::operator!=(const MathVector<T, U>& mv){
    for (int i = 0; i<U; i++){
        if (this->fields[i] != mv.fields[i]) return true;
    }
    return false;
}

template <typename T, size_t U>
ostream& operator << (ostream & os, const MathVector<T, U> & mv) {
    os <<"[ ";
    for (int i = 0; i<U; i++){
        os << mv.fields[i] << " ";
    }
    return (os << "]");
}

template <typename T, size_t U>
istream& operator >> (istream & is, const MathVector<T, U> & mv) {
    for (int i = 0; i<U; i++){
        is >> mv.fields[i];
    }
    return is;
}