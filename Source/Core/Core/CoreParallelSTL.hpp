#pragma once

#include "CoreInclude.h"

template<typename T>
CoreParallelSTL<T>::CoreParallelSTL(const size_t newCapacity) : capacity(newCapacity)
{
}

template<typename T>
void CoreParallelSTL<T>::Push(void)
{
	SetSize(this->size + 1); 
}

template<typename T>
void CoreParallelSTL<T>::Pop(void)
{
	SetSize(this->size - 1); 
}

template<typename T>
void CoreParallelSTL<T>::Clear(void)
{
	SetSize(0); 
};

template<typename T>
size_t CoreParallelSTL<T>::GetCapacity(void)
{
	READ_LOCK(this->mutex);
	return this->capacity;
}

template<typename T>
size_t CoreParallelSTL<T>::GetSize(void)
{
	READ_LOCK(this->mutex);
	return this->size;
}

template<typename T>
void CoreParallelSTL<T>::SetSize(const size_t newSize)
{
	WRITE_LOCK(mutex);
	this->size = newSize;
}

template<typename T>
void CoreParallelSTL<T>::SetCapacity(const size_t newCapacity)
{
	WRITE_LOCK(mutex);
	this->capacity = newCapacity;
}