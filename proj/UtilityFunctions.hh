#pragma once

#include <cmath>

namespace utility {
    

template <typename T>
inline void CustomSwap(T & a, T & b){
    T p = a;
    a = b;
    b = p;
}

template  <typename T>
inline T RandomT(const T & _min, const T & _max){
    if (_min == _max) return _min;
    return _min + static_cast <T> (rand()) /( static_cast <T> (RAND_MAX/(_max-_min)));
}

template  <typename T>
inline T* RandomTList(const int & _size, const T & _min, const T & _max){
    T* out = new T [_size];
    for (int i = 0; i < _size; i++){
        out[i] = RandomFloat(_min, _max);
    }
    return out;
}

template  <typename T, typename U>
inline int partition(T* structures, U* keys, const int & start, const int & end, const bool & ascendingOrder)
{
    int startPivot = (start + end) / 2;
    U pivot = keys[startPivot];
 
    int count = 0;
    for (int i = start; i <= end; i++) {
        if (i != startPivot && ((keys[i] <= pivot && ascendingOrder) || (keys[i] > pivot && !ascendingOrder)))
            count++;
    }
 
    // Giving pivot element its correct position
    int pivotIndex = start + count;
    CustomSwap(keys[pivotIndex], keys[startPivot]);
    CustomSwap(structures[pivotIndex], structures[startPivot]);
 
    // Sorting left and right parts of the pivot element
    int i = start, j = end;
 
    while (i < pivotIndex && j > pivotIndex) {
 
        while ((keys[i] <= pivot && ascendingOrder) || (keys[i] > pivot && !ascendingOrder)) {
            i++;
        }
 
        while ((keys[j] > pivot && ascendingOrder) || (keys[j] <= pivot && !ascendingOrder)) {
            j--;
        }
 
        if (i < pivotIndex && j > pivotIndex) {
            CustomSwap(keys[i], keys[j]);
            CustomSwap(structures[i], structures[j]);
            i++; j--;
        }
    }
 
    return pivotIndex;
}
 
template  <typename T, typename U>
inline void quickSort(T* structures, U* keys, const int & start, const int & end, const bool & ascendingOrder)
{
 
    // base case
    if (start >= end)
        return;
 
    // partitioning the array
    int p = partition<T, U>(structures, keys, start, end, ascendingOrder);
 
    // Sorting the left part
    quickSort<T, U>(structures, keys, start, p - 1, ascendingOrder);
 
    // Sorting the right part
    quickSort<T, U>(structures, keys, p + 1, end, ascendingOrder);
}

template  <typename T, typename U>
inline void QuickSortStructuresByKey(const int & siz, T* structures, U* keys, const bool & ascendingOrder){
    quickSort(structures, keys, 0, siz - 1, ascendingOrder);
}


template  <typename T, typename U>
inline void BubbleSortStructuresByKey(const int & siz, T* structures, U* keys, const bool & ascendingOrder){
    for (int j = 0; j < siz; j++){
        for (int i = 0; i < siz - j - 1; i++){
            if ((ascendingOrder && keys[i] > keys[i+1]) || (!ascendingOrder && keys[i] < keys[i+1])){
                CustomSwap(keys[i], keys[i+1]);
                CustomSwap(structures[i], structures[i+1]);
            }
        }
    }
}

} // namespace utility