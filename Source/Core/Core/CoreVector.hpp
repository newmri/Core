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
	{
		SetCapacity(maxBlockNum);
		CoreContainer<T>::SetSize(0);
	}
}

template<typename T>
void CoreVector<T>::Init(void)
{
	CoreContainer<T>::Init();
}

template<typename T>
CoreVector<T>& CoreVector<T>::operator=(const CoreVector<T>& rhs)
{
	Copy(rhs);
	return *this;
}

template<typename T>
CORE_OUT(T) CoreVector<T>::operator[](const size_t index)
{
	return this->data[index];
}

template<typename T>
CORE_REF(T) CoreVector<T>::operator[](const size_t index) const
{
	return this->operator[](index);
}

template<typename T>
void CoreVector<T>::Copy(const CoreVector<T>& rhs)
{
	clear();

	WRITE_LOCK(this->mutex);
	this->data = CoreContainer<T>::Alloc(rhs.dataCapacity, rhs.dataSize);

	if (this->data)
	{
		memcpy_s(this->data, sizeof(T) * this->dataSize, rhs.data, sizeof(T) * rhs.dataSize);
	}
}

template<typename T>
void CoreVector<T>::clear(void)
{
	WRITE_LOCK(this->mutex);
	CoreContainer<T>::DeAlloc();
	this->dataCapacity = 0;
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
	this->data = CoreContainer<T>::Alloc(newCapacity, newCapacity, false);

	if (this->data)
	{
		SetCapacity(newCapacity);
		CoreContainer<T>::SetSize(0);
	}
}

template<typename T>
void CoreVector<T>::push_back(const T& data)
{
	WRITE_LOCK(this->mutex);

	if (this->dataSize == this->dataCapacity)
	{
		// 재할당
	}

	new(&this->data[this->dataSize]) T(data);
	//memcpy_s(&this->data[this->dataSize], sizeof(T), &data, sizeof(T));
	++this->dataSize;
}

template<typename T>
void CoreVector<T>::push_back(T&& data)
{
	WRITE_LOCK(this->mutex);

	if (this->dataSize == this->dataCapacity)
	{
		// 재할당
	}

	new(&this->data[this->dataSize]) T(std::move(data));
	++this->dataSize;
}

template<typename T>
void CoreVector<T>::pop_back(void)
{
	WRITE_LOCK(this->mutex);

	if (this->dataSize)
		CoreContainer<T>::SetSize(this->dataSize - 1);
}

template<typename T>
void CoreVector<T>::SetCapacity(const size_t newCapacity)
{
	WRITE_LOCK(this->mutex);
	this->dataCapacity = newCapacity;
}
