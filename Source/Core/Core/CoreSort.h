#pragma once

template<typename T>
void CoreSwap(T& left, T& right);

template<typename T>
bool CoreAscendingOrder(const T& left, const T& right);

template<typename T>
bool CoreDescendingOrder(const T& left, const T& right);

template<typename T>
size_t CoreGetLen(T* start, T* end);

template<typename T, typename FUNC = bool(*)(const T&, const T&)>
void CoreBubbleSort(T* start, T* end, FUNC Compare);

template<typename T>
void CoreBubbleSort(T* start, T* end);

template<typename T, typename FUNC = bool(*)(const T&, const T&)>
void CoreSelectionSort(T* start, T* end, FUNC Compare);

template<typename T>
void CoreSelectionSort(T* start, T* end);

template<typename T, typename FUNC = bool(*)(const T&, const T&)>
void CoreInsertionSort(T* start, T* end, FUNC Compare);

template<typename T>
void CoreInsertionSort(T* start, T* end);

#include "CoreSort.hpp"