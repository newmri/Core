#pragma once

#include "CoreSharedPtr.h"

template<typename T>
std::atomic<size_t> CoreSharedPtr<T>::refNum = 0;

template<typename T>
CoreSharedPtr<T>::CoreSharedPtr() : blockBody(nullptr), allocatedBlockNum(0)
{

}

template<typename T>
CoreSharedPtr<T>::CoreSharedPtr(const CoreSharedPtr<T>& other) : allocatedBlockNum(other.allocatedBlockNum), blockBody(other.blockBody)
{
	++this->refNum;
}

template<typename T>
CoreSharedPtr<T>::CoreSharedPtr(T* blockBody, const size_t needBlockNum) : blockBody(blockBody), allocatedBlockNum(needBlockNum)
{
	++this->refNum;
}

template<typename T>
CoreSharedPtr<T>::~CoreSharedPtr()
{
	if (IS_SAME(0, this->allocatedBlockNum))
		return;

	if (IS_NOT_SAME(0, --this->refNum))
		return;

	if(IS_SAME(1, allocatedBlockNum))
		delete this->blockBody;
	else
		delete[] this->blockBody;

	this->blockBody = nullptr;
}

template<typename T>
T* CoreSharedPtr<T>::get(void)
{
	return &this->blockBody;
}

template<typename T>
bool CoreSharedPtr<T>::IsValid(void)
{
	if (IS_NULL(this->blockBody))
		return false;

	return this->blockBody->IsValid(blockBody);
}

template<typename T>
T* CoreSharedPtr<T>::operator->()
{
	return this->blockBody;
}

template<typename T>
T& CoreSharedPtr<T>::operator*()
{
	return *this->blockBody;
}

template<typename T>
T& CoreSharedPtr<T>::operator[] (const size_t index)
{
	return this->blockBody[index];
}

template<typename T>
const T& CoreSharedPtr<T>::operator[](const size_t index) const
{
	return this->blockBody[index];
}

template<typename T>
CoreSharedPtr<T>& CoreSharedPtr<T>::operator=(const CoreSharedPtr<T>& other)
{
	this->allocatedBlockNum = other.allocatedBlockNum;
	this->blockBody = other.blockBody;

	++this->refNum;

	return *this;
}

template<typename T>
CoreSharedPtr<T>::operator bool() const
{
	return (IS_NOT_NULL(this->blockBody));
}