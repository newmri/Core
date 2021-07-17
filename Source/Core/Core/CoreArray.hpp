#pragma once

#include "CoreArray.h"

template<typename T, const size_t N>
CoreArray<T, N>::CoreArray()
{
	Init();
}

template<typename T, const size_t N>
CoreArray<T, N>::CoreArray(std::initializer_list<T> list)
{
	auto iterBegin = list.begin();
	auto iterEnd = list.end();

	for (size_t i = 0; iterBegin != iterEnd; ++iterBegin, ++i)
	{
		this->data[i] = *iterBegin;
	}
}

template<typename T, const size_t N>
CoreArray<T, N>::CoreArray(CoreArray<T, N>& rhs)
{
	Copy(rhs);
}

template<typename T, const size_t N>
CoreArray<T, N>::~CoreArray()
{

}

template<typename T, const size_t N>
void CoreArray<T, N>::Init(void)
{
	memset(this->data, 0, sizeof(T) * N);
}

template<typename T, const size_t N>
CoreArray<T, N>& CoreArray<T, N>::operator=(const CoreArray<T, N>& rhs)
{
	Copy(rhs);
	return *this;
}

template<typename T, const size_t N>
CORE_OUT(T) CoreArray<T, N>::operator[](const size_t index)
{
	return this->data[index];
}

template<typename T, const size_t N>
CORE_REF(T) CoreArray<T, N>::operator[](const size_t index) const
{
	return this->operator[](index);
}

template<typename T, const size_t N>
size_t CoreArray<T, N>::size(void) const
{
	return N;
}

template<typename T, const size_t N>
void CoreArray<T, N>::Copy(const CoreArray<T, N>& rhs)
{
	WRITE_LOCK(this->mutex);
	memcpy_s(this->data, sizeof(T) * N, rhs.data, sizeof(T) * N);
}