#pragma once

#include "CoreParallelArray.h"

template<typename T>
CoreParallelArray<T>::CoreParallelArray(const size_t size) : CoreParallelSTL<T>::CoreParallelSTL(size)
{
	this->data = GET_INSTANCE(CoreMemoryPoolManager<T>).Alloc(size, size);
	CoreParallelSTL<T>::SetSize(size);
}

template<typename T>
CoreParallelArray<T>::~CoreParallelArray()
{
	Clear();
}

template<typename T>
void CoreParallelArray<T>::Clear(void)
{
	WRITE_LOCK(this->mutex);
	GET_INSTANCE(CoreMemoryPoolManager<T>).DeAlloc(this->data);
	CoreParallelSTL<T>::Clear();
}