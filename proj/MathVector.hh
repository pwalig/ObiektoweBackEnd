#pragma once

#include <cmath> // sqrt()

#include <iostream>


namespace linear_algebra {


//----------Vector Declarations----------

template <typename T, size_t U>
class Vector {
    public:
    T* components;

    Vector();
    template <typename V> Vector(const V & init);
    Vector(const Vector & other);
    Vector(Vector && other);
    ~Vector();

    Vector& operator=(const Vector& other);
    Vector& operator=(Vector&& other);
    Vector& operator+=(const Vector<T, U> & mv);
    Vector& operator-=(const Vector<T, U> & mv);
    template <typename V> Vector& operator*=(const V & va);
    template <typename V> Vector& operator/=(const V & va);
    Vector operator+(const Vector<T, U>& mv);
    Vector operator-(const Vector<T, U>& mv);
    Vector operator-();
    template <typename V> Vector operator*(const V & va);
    template <typename V> Vector operator/(const V & va);
    bool operator==(const Vector<T, U>& mv);
    bool operator!=(const Vector<T, U>& mv);
    T& operator[](const int & id);

    T GetMagnitude();
    Vector GetNormalized();
    void Normalize();

    static T DotProduct(const Vector<T, U> & mv1, const Vector<T, U> & mv2);
};

template <typename T>
class Vector2 {
    public:
    T x;
    T y;

    Vector2() = default;
    template <typename V> Vector2(const V & init);
    template <typename V> Vector2(const V & _x, const V & _y);

    Vector2& operator+=(const Vector2<T> & mv);
    Vector2& operator-=(const Vector2<T> & mv);
    template <typename V> Vector2& operator*=(const V & va);
    template <typename V> Vector2& operator/=(const V & va);
    Vector2 operator+(const Vector2<T>& mv);
    Vector2 operator-(const Vector2<T>& mv);
    Vector2 operator-();
    template <typename V> Vector2 operator*(const V & va);
    template <typename V> Vector2 operator/(const V & va);
    bool operator==(const Vector2<T>& mv);
    bool operator!=(const Vector2<T>& mv);

    T GetMagnitude();
    Vector2 GetNormalized();
    void Normalize();
    
    static Vector2 CrossProduct(const Vector2<T> & mv);
    static T DotProduct(const Vector2<T> & mv1, const Vector2<T> & mv2);
};

template <typename T>
class Vector3 {
    public:
    T x;
    T y;
    T z;

    Vector3() = default;
    template <typename V> Vector3(const V & init);
    template <typename V> Vector3(const V & _x, const V & _y, const V & _z);

    Vector3& operator+=(const Vector3<T> & mv);
    Vector3& operator-=(const Vector3<T> & mv);
    template <typename V> Vector3& operator*=(const V & va);
    template <typename V> Vector3& operator/=(const V & va);
    Vector3 operator+(const Vector3<T>& mv);
    Vector3 operator-(const Vector3<T>& mv);
    Vector3 operator-();
    template <typename V> Vector3 operator*(const V & va);
    template <typename V> Vector3 operator/(const V & va);
    bool operator==(const Vector3<T>& mv);
    bool operator!=(const Vector3<T>& mv);

    T GetMagnitude();
    Vector3 GetNormalized();
    void Normalize();

    static Vector3 CrossProduct(const linear_algebra::Vector3<T> & mv1, const linear_algebra::Vector3<T> & mv2);
    static T DotProduct(const Vector3<T> & mv1, const Vector3<T> & mv2);
};


//----------Function Declarations----------

template<typename T, size_t U>
void swap(Vector<T, U> & a, Vector<T, U> & b);

template<typename T>
void swap(Vector2<T> & a, Vector2<T> & b);

template<typename T>
void swap(Vector3<T> & a, Vector3<T> & b);

template <typename T>
Vector<T, 2> CrossProduct2(const Vector<T, 2> & mv);

template <typename T>
Vector<T, 3> CrossProduct3(const Vector<T, 3> & mv1, const Vector<T, 3> & mv2);


} // namespace linear_algebra


//----------Constructor Definitions----------

template <typename T, size_t U>
inline linear_algebra::Vector<T, U>::Vector(){
    components = new T[U];
}

template <typename T, size_t U> template <typename V>
inline linear_algebra::Vector<T, U>::Vector(const V & init){
    components = new T[U];
    for (int i = 0; i < U; i++)
        components[i] = init;
}

template <typename T, size_t U>
inline linear_algebra::Vector<T, U>::Vector(const Vector & other) : components(nullptr){
    components = new T[U];
    for (int i = 0; i < U; i++)
        components[i] = other.components[i];
}

template <typename T, size_t U>
inline linear_algebra::Vector<T, U>::Vector(Vector && other) : components(other.components){
    other.components = nullptr;
}

template <typename T, size_t U>
inline linear_algebra::Vector<T, U>::~Vector(){
    delete components;
}

template <typename T> template <typename V>
inline linear_algebra::Vector2<T>::Vector2(const V & init) : x(init), y(init) { }

template <typename T> template <typename V>
inline linear_algebra::Vector2<T>::Vector2(const V & _x, const V & _y) : x(_x), y(_y) { }

template <typename T> template <typename V>
inline linear_algebra::Vector3<T>::Vector3(const V & init) : x(init), y(init), z(init) { }

template <typename T> template <typename V>
inline linear_algebra::Vector3<T>::Vector3(const V & _x, const V & _y, const V & _z) : x(_x), y(_y), z(_z) { }


//----------Function Definitions----------

template<typename T, size_t U>
inline void linear_algebra::swap(linear_algebra::Vector<T, U> & a, linear_algebra::Vector<T, U> & b){
    T* comp = a.components;
    a.components = b.components;
    b.components = comp;
}

template<typename T>
inline void linear_algebra::swap(linear_algebra::Vector2<T> & a, linear_algebra::Vector2<T> & b){
    Vector2<T> comp = a;
    a = b;
    b = comp;
}

template<typename T>
inline void linear_algebra::swap(linear_algebra::Vector3<T> & a, linear_algebra::Vector3<T> & b){
    Vector3<T> comp = a;
    a = b;
    b = comp;
}

template <typename T>
inline linear_algebra::Vector<T, 2> linear_algebra::CrossProduct2(const linear_algebra::Vector<T, 2> & mv){
    Vector<T, 2> r;
    r.components[0] = -mv.components[1];
    r.components[1] = mv.components[0];
    return r;
}

template <typename T>
inline linear_algebra::Vector<T, 3> linear_algebra::CrossProduct3(const linear_algebra::Vector<T, 3> & mv1, const linear_algebra::Vector<T, 3> & mv2){
    Vector<T, 3> r;
    r.components[0] = (mv1.components[1] * mv2.components[2]) - (mv1.components[2] * mv2.components[1]);
    r.components[1] = (mv1.components[2] * mv2.components[0]) - (mv1.components[0] * mv2.components[2]);
    r.components[2] = (mv1.components[0] * mv2.components[1]) - (mv1.components[1] * mv2.components[0]);
    return r;
}


//----------Method Definitions----------

//----------Vector----------

template <typename T, size_t U>
inline T linear_algebra::Vector<T, U>::GetMagnitude(){
    T sum = 0;
    for (int i = 0; i<U; i++){
        sum += (this->components[i] * this->components[i]);
    }
    return sqrt(sum);
}

template <typename T, size_t U>
inline linear_algebra::Vector<T, U> linear_algebra::Vector<T, U>::GetNormalized(){
    Vector<T, U> r;
    T magnitude = this->GetMagnitude();
    for (int i = 0; i<U; ++i){
        r.components[i] = this->components[i] / magnitude;
    }
    return r;
}

template <typename T, size_t U>
inline void linear_algebra::Vector<T, U>::Normalize(){
    T magnitude = this->GetMagnitude();
    for (int i = 0; i<U; ++i){
        this->components[i] /= magnitude;
    }
}

template <typename T, size_t U>
inline T linear_algebra::Vector<T, U>::DotProduct(const linear_algebra::Vector<T, U> & mv1, const linear_algebra::Vector<T, U> & mv2){
    T r = 0;
    for (int i = 0; i<U; i++){
        r += mv1.components[i] * mv2.components[i];
    }
    return r;
}

//----------Vector2----------

template <typename T>
inline T linear_algebra::Vector2<T>::GetMagnitude(){
    T sum = (x*x) + (y*y);
    return sqrt(sum);
}

template <typename T>
inline linear_algebra::Vector2<T> linear_algebra::Vector2<T>::GetNormalized(){
    T magnitude = this->GetMagnitude();
    Vector2<T> r(this->x / magnitude, this->y / magnitude);
    return r;
}

template <typename T>
inline void linear_algebra::Vector2<T>::Normalize(){
    T magnitude = this->GetMagnitude();
    this->x /= magnitude;
    this->y /= magnitude;
}

template <typename T>
inline linear_algebra::Vector2<T> linear_algebra::Vector2<T>::CrossProduct(const linear_algebra::Vector2<T> & mv){
    Vector2<T> r;
    r.x = -mv.y;
    r.y = mv.x;
    return r;
}

template <typename T>
inline T linear_algebra::Vector2<T>::DotProduct(const linear_algebra::Vector2<T> & mv1, const linear_algebra::Vector2<T> & mv2){
    T r = 0;
    r += mv1.x * mv2.x;
    r += mv1.y * mv2.y;
    return r;
}

//----------Vector3----------

template <typename T>
inline T linear_algebra::Vector3<T>::GetMagnitude(){
    T sum = (x*x) + (y*y) + (z*z);
    return sqrt(sum);
}

template <typename T>
inline linear_algebra::Vector3<T> linear_algebra::Vector3<T>::GetNormalized(){
    T magnitude = this->GetMagnitude();
    Vector3<T> r(this->x / magnitude, this->y / magnitude);
    return r;
}

template <typename T>
inline void linear_algebra::Vector3<T>::Normalize(){
    T magnitude = this->GetMagnitude();
    this->x /= magnitude;
    this->y /= magnitude;
}

template <typename T>
inline linear_algebra::Vector3<T> linear_algebra::Vector3<T>::CrossProduct(const linear_algebra::Vector3<T> & mv1, const linear_algebra::Vector3<T> & mv2){
    Vector3<T> r;
    r.x = (mv1.y * mv2.z) - (mv1.z * mv2.y);
    r.y = (mv1.z * mv2.x) - (mv1.x * mv2.z);
    r.z = (mv1.x * mv2.y) - (mv1.y * mv2.x);
    return r;
}

template <typename T>
inline T linear_algebra::Vector3<T>::DotProduct(const linear_algebra::Vector3<T> & mv1, const linear_algebra::Vector3<T> & mv2){
    T r = 0;
    r += mv1.x * mv2.x;
    r += mv1.y * mv2.y;
    r += mv1.z * mv2.z;
    return r;
}


//----------Operator Definitions----------

//----------Vector----------

template <typename T, size_t U>
inline linear_algebra::Vector<T, U>& linear_algebra::Vector<T, U>::operator=(const Vector& other){
    if (this != &other){
        for (int i = 0; i < U; i++){
            this->components[i] = other.components[i];
        }
    }
    return *this;
}
template <typename T, size_t U>
inline linear_algebra::Vector<T, U>& linear_algebra::Vector<T, U>::operator=(Vector&& other){
    if (this != &other){
        delete components;
        this->components = other.components;
        other.components = nullptr;
    }
    return *this;
}

template <typename T, size_t U>
inline linear_algebra::Vector<T, U>& linear_algebra::Vector<T, U>::operator+=(const linear_algebra::Vector<T, U> & mv){
    for (int i = 0; i < U; i++) {
        this->components[i] += mv.components[i];
    }
    return *this;
}
template <typename T, size_t U>
inline linear_algebra::Vector<T, U>& linear_algebra::Vector<T, U>::operator-=(const linear_algebra::Vector<T, U> & mv){
    for (int i = 0; i < U; i++) {
        this->components[i] -= mv.components[i];
    }
    return *this;
}
template <typename T, size_t U> template <typename V>
inline linear_algebra::Vector<T, U>& linear_algebra::Vector<T, U>::operator*=(const V & va){
    for (int i = 0; i < U; i++) {
        this->components[i] *= va;
    }
    return *this;
}
template <typename T, size_t U> template <typename V> 
inline linear_algebra::Vector<T, U>& linear_algebra::Vector<T, U>::operator/=(const V & va){
    for (int i = 0; i < U; i++) {
        this->components[i] /= va;
    }
    return *this;
}
template <typename T, size_t U>
inline linear_algebra::Vector<T, U> linear_algebra::Vector<T, U>::operator+(const linear_algebra::Vector<T, U>& mv) {
    Vector<T,U> r;
    for (int i = 0; i<U; i++) {
        r.components[i] = this->components[i] + mv.components[i];
    }
    return r;
}
template <typename T, size_t U>
inline linear_algebra::Vector<T, U> linear_algebra::Vector<T, U>::operator-(const linear_algebra::Vector<T, U>& mv){
    Vector<T,U> r;
    for (int i = 0; i<U; i++) {
        r.components[i] = this->components[i] - mv.components[i];
    }
    return r;
}
template <typename T, size_t U>
inline linear_algebra::Vector<T, U> linear_algebra::Vector<T, U>::operator-(){
    Vector<T,U> r;
    for (int i = 0; i<U; i++) {
        r.components[i] = -this->components[i];
    }
    return r;
}
template <typename T, size_t U> template <typename V>
inline linear_algebra::Vector<T, U> linear_algebra::Vector<T, U>::operator*(const V & va){
    Vector<T,U> r;
    for (int i = 0; i<U; i++) {
        r.components[i] = this->components[i] * va;
    }
    return r;
}
template <typename T, size_t U> template <typename V>
inline linear_algebra::Vector<T, U> linear_algebra::Vector<T, U>::operator/(const V & va){
    Vector<T,U> r;
    for (int i = 0; i<U; i++) {
        r.components[i] = this->components[i] / va;
    }
    return r;
}
template <typename T, size_t U>
inline bool linear_algebra::Vector<T, U>::operator==(const linear_algebra::Vector<T, U>& mv){
    for (int i = 0; i<U; i++){
        if (this->components[i] != mv.components[i]) return false;
    }
    return true;
}
template <typename T, size_t U>
inline bool linear_algebra::Vector<T, U>::operator!=(const linear_algebra::Vector<T, U>& mv){
    for (int i = 0; i<U; i++){
        if (this->components[i] != mv.components[i]) return true;
    }
    return false;
}

template <typename T, size_t U, typename V>
inline linear_algebra::Vector<T, U> operator*(const V & va, const linear_algebra::Vector<T, U> & mv){
    linear_algebra::Vector<T, U> r;
    for (int i = 0; i<U; i++) {
        r.components[i] = mv.components[i] * va;
    }
    return r;
}

template <typename T, size_t U>
inline T& linear_algebra::Vector<T, U>::operator[](const int & id){
    return components[id];
}

template <typename T, size_t U>
inline std::ostream& operator << (std::ostream & os, const linear_algebra::Vector<T, U> & mv) {
    os <<"[ ";
    for (int i = 0; i<U; i++){
        os << mv.components[i] << " ";
    }
    return (os << "]");
}

template <typename T, size_t U>
inline std::istream& operator >> (std::istream & is, const linear_algebra::Vector<T, U> & mv) {
    for (int i = 0; i<U; i++){
        is >> mv.components[i];
    }
    return is;
}

//----------Vector2----------

template <typename T>
inline linear_algebra::Vector2<T>& linear_algebra::Vector2<T>::operator+=(const Vector2<T> & mv){
    this->x += mv.x;
    this->y += mv.y;
    return *this;
}
template <typename T>
inline linear_algebra::Vector2<T>& linear_algebra::Vector2<T>::operator-=(const Vector2<T> & mv){
    this->x -= mv.x;
    this->y -= mv.y;
    return *this;
}

template <typename T> template <typename V>
inline linear_algebra::Vector2<T>& linear_algebra::Vector2<T>::operator*=(const V & va){
    this->x *= va;
    this->y *= va;
    return *this;
}

template <typename T> template <typename V>
inline linear_algebra::Vector2<T>& linear_algebra::Vector2<T>::operator/=(const V & va){
    this->x /= va;
    this->y /= va;
    return *this;
}

template <typename T>
inline linear_algebra::Vector2<T> linear_algebra::Vector2<T>::operator+(const Vector2<T>& mv){
    Vector2<T> r(this->x + mv.x, this->y + mv.y);
    return r;
}

template <typename T>
inline linear_algebra::Vector2<T> linear_algebra::Vector2<T>::operator-(const Vector2<T>& mv){
    Vector2<T> r(this->x - mv.x, this->y - mv.y);
    return r;
}

template <typename T>
inline linear_algebra::Vector2<T> linear_algebra::Vector2<T>::operator-(){
    Vector2<T> r(-(this->x), -(this->y));
    return r;
}

template <typename T> template <typename V>
inline linear_algebra::Vector2<T> linear_algebra::Vector2<T>::operator*(const V & va){
    Vector2<T> r(this->x * va, this->y * va);
    return r;
}

template <typename T> template <typename V>
inline linear_algebra::Vector2<T> linear_algebra::Vector2<T>::operator/(const V & va){
    Vector2<T> r(this->x / va, this->y / va);
    return r;
}

template <typename T>
inline bool linear_algebra::Vector2<T>::operator==(const Vector2<T>& mv){
    if (this->x == mv.x && this->y == mv.y) return true;
    return false;
}

template <typename T>
inline bool linear_algebra::Vector2<T>::operator!=(const Vector2<T>& mv){
    if (this->x == mv.x && this->y == mv.y) return false;
    return true;
}

template <typename T, typename V>
inline linear_algebra::Vector2<T> operator*(const V & va, const linear_algebra::Vector2<T> & mv){
    linear_algebra::Vector2<T> r(va * mv.x, va * mv.y);
    return r;
}

template <typename T>
inline std::ostream& operator << (std::ostream & os, const linear_algebra::Vector2<T> & mv) {
    os << "[ " << mv.x << " " << mv.y << " ]";
    return os;
}

template <typename T>
inline std::istream& operator >> (std::istream & is, const linear_algebra::Vector2<T> & mv) {
    is >> mv.x >> mv.y;
    return is;
}

//----------Vector3----------

template <typename T>
inline linear_algebra::Vector3<T>& linear_algebra::Vector3<T>::operator+=(const Vector3<T> & mv){
    this->x += mv.x;
    this->y += mv.y;
    this->z += mv.z;
    return *this;
}
template <typename T>
inline linear_algebra::Vector3<T>& linear_algebra::Vector3<T>::operator-=(const Vector3<T> & mv){
    this->x -= mv.x;
    this->y -= mv.y;
    this->z -= mv.z;
    return *this;
}

template <typename T> template <typename V>
inline linear_algebra::Vector3<T>& linear_algebra::Vector3<T>::operator*=(const V & va){
    this->x *= va;
    this->y *= va;
    this->z *= va;
    return *this;
}

template <typename T> template <typename V>
inline linear_algebra::Vector3<T>& linear_algebra::Vector3<T>::operator/=(const V & va){
    this->x /= va;
    this->y /= va;
    this->z /= va;
    return *this;
}

template <typename T>
inline linear_algebra::Vector3<T> linear_algebra::Vector3<T>::operator+(const Vector3<T>& mv){
    Vector3<T> r(this->x + mv.x, this->y + mv.y, this->z + mv.z);
    return r;
}

template <typename T>
inline linear_algebra::Vector3<T> linear_algebra::Vector3<T>::operator-(const Vector3<T>& mv){
    Vector3<T> r(this->x - mv.x, this->y - mv.y, this->z - mv.z);
    return r;
}

template <typename T>
inline linear_algebra::Vector3<T> linear_algebra::Vector3<T>::operator-(){
    Vector3<T> r(-this->x, -this->y, -this->z);
    return r;
}

template <typename T> template <typename V>
inline linear_algebra::Vector3<T> linear_algebra::Vector3<T>::operator*(const V & va){
    Vector3<T> r(this->x * va, this->y * va, this->z * va);
    return r;
}

template <typename T> template <typename V>
inline linear_algebra::Vector3<T> linear_algebra::Vector3<T>::operator/(const V & va){
    Vector3<T> r(this->x / va, this->y / va, this->z / va);
    return r;
}

template <typename T>
inline bool linear_algebra::Vector3<T>::operator==(const Vector3<T>& mv){
    if (this->x == mv.x && this->y == mv.y && this->z == mv.z) return true;
    return false;
}

template <typename T>
inline bool linear_algebra::Vector3<T>::operator!=(const Vector3<T>& mv){
    if (this->x == mv.x && this->y == mv.y && this->z == mv.z) return false;
    return true;
}

template <typename T, typename V>
inline linear_algebra::Vector3<T> operator*(const V & va, const linear_algebra::Vector3<T> & mv){
    linear_algebra::Vector3<T> r(va * mv.x, va * mv.y, va * mv.z);
    return r;
}

template <typename T>
inline std::ostream& operator << (std::ostream & os, const linear_algebra::Vector3<T> & mv) {
    os << "[ " << mv.x << " " << mv.y << " " << mv.z << " ]";
    return os;
}

template <typename T>
inline std::istream& operator >> (std::istream & is, linear_algebra::Vector3<T> & mv) {
    is >> mv.x >> mv.y >> mv.z;
    return is;
}