#pragma once

#include "CoreVector.h"

template<typename T>
CoreVector<T>::CoreVector()
{
	reserve(this->dataDefaultReserveSize);
}

template<typename T>
CoreVector<T>::~CoreVector()
{
	SAFE_DELETE(this->dataCapacity, this->data);
	SetCapacity(0);
	CoreContainer<T>::SetSize(0);
}

template<typename T>
template<typename... Types>
CoreVector<T>::CoreVector(const size_t maxBlockNum, Types... args) : CoreContainer<T>::CoreContainer(maxBlockNum, maxBlockNum, args...)
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
CoreVector<T>::CoreVector(CoreVector<T>& rhs)
{
	READ_LOCK(rhs.mutex);
	Copy(rhs);
}

template<typename T>
CoreVector<T>& CoreVector<T>::operator=(CoreVector<T>& rhs)
{
	READ_LOCK(rhs.mutex);
	Copy(rhs);
	return *this;
}

template<typename T>
const T& CoreVector<T>::operator[](const size_t index) const
{
	return this->data[index];
}

template<typename T>
T& CoreVector<T>::operator[](const size_t index)
{
	return const_cast<T&>(static_cast<const CoreVector<T>&>(*this)[index]);
}

template<typename T>
void CoreVector<T>::Copy(const CoreVector<T>& rhs)
{
	clear();

	WRITE_LOCK(this->mutex);

	if (1 < rhs.dataCapacity)
		this->data = new T[rhs.dataCapacity];
	else
		this->data = new T;

	if (this->data)
	{
		size_t copySize = sizeof(T) * rhs.dataSize;
		memcpy_s(this->data, copySize, rhs.data, copySize);

		SetCapacity(rhs.dataCapacity);
		CoreContainer<T>::SetSize(rhs.dataSize);

		this->dataDefaultReserveSize = rhs.dataDefaultReserveSize;
		this->dataCapacityIncrease = rhs.dataCapacityIncrease;

	}
}

template<typename T>
void CoreVector<T>::clear(void)
{
	WRITE_LOCK(this->mutex);

	for (size_t i = 0; i < this->dataSize; ++i)
	{
		this->data[i].~T();
	}

	CoreContainer<T>::SetSize(0);
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
	WRITE_LOCK(this->mutex);

	if(1 < newCapacity)
		this->data = new T[newCapacity];
	else
		this->data = new T;

	if (this->data)
	{
		SetCapacity(newCapacity);
	}
}

template<typename T>
void CoreVector<T>::push_back(const T& data)
{
	WRITE_LOCK(this->mutex);

	CheckCapacityAndReAlloc();
	new(&this->data[this->dataSize]) T(data);
	CoreContainer<T>::SetSize(this->dataSize + 1);
}

template<typename T>
void CoreVector<T>::push_back(T&& data)
{
	WRITE_LOCK(this->mutex);

	CheckCapacityAndReAlloc();
	new(&this->data[this->dataSize]) T(std::move(data));
	CoreContainer<T>::SetSize(this->dataSize + 1);
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
	this->dataCapacity = newCapacity;
}

template<typename T>
void CoreVector<T>::CheckCapacityAndReAlloc(void)
{
	if (IS_SAME(this->dataSize, this->dataCapacity))
	{
		size_t oldCapacity = this->dataCapacity;
		T* newData = ReAlloc();

		if (IS_NULL(newData))
			return; // assert ÇÊ¿ä

		SAFE_DELETE(oldCapacity, this->data);

		this->data = newData;
	}
}

template<typename T>
T* CoreVector<T>::ReAlloc(void)
{
	size_t newCapacity = this->dataCapacity * this->dataCapacityIncrease;
	size_t oldSize = this->dataSize;
	size_t copySize = sizeof(T) * oldSize;

	T* newData = new T[newCapacity];

	if (newData)
	{
		memcpy_s(newData, copySize, this->data, copySize);

		SetCapacity(newCapacity);

		return newData;
	}

	return nullptr;
}
