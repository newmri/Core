#pragma once

#include "CoreParallelArray.h"

template<typename T>
CoreParallelArray<T>::CoreParallelArray(const size_t newCapacity) : CoreParallelSTL<T>::CoreParallelArray(newCapacity)
{
	
}

template<typename T>
void CoreParallelArray<T>::Push(const T newData)
{

	CoreParallelSTL<T>::Push();
}

template<typename T>
void CoreParallelArray<T>::Pop(void)
{

	CoreParallelSTL<T>::Pop();
}

template<typename T>
void CoreParallelArray<T>::Clear(void)
{

	CoreParallelSTL<T>::Clear();
}