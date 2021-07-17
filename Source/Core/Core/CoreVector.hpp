#pragma once

#include "CoreVector.h"

template<typename T>
CoreVector<T>::CoreVector()
{
}

template<typename T>
CoreVector<T>::~CoreVector()
{

}

template<typename T>
template<typename... Types>
CoreVector<T>::CoreVector(const size_t maxBlockNum, const size_t needBlockNum, Types... args) : CoreContainer<T>::CoreContainer(maxBlockNum, needBlockNum, args...)
{
	if (this->data)
		this->dataCapacity = maxBlockNum;
}

template<typename T>
void CoreVector<T>::Init(void)
{
	CoreContainer<T>::Init();
}

template<typename T>
void CoreVector<T>::Clear(void)
{
}

template<typename T>
size_t CoreVector<T>::capacity(void)
{
	READ_LOCK(this->mutex);
	return this->dataCapacity;
}

template<typename T>
void CoreVector<T>::reserve(const size_t newCapacity)
{
	this->data = CoreContainer<T>::Alloc(newCapacity, 0);

	if (this->data)
		this->dataCapacity = newCapacity;
}