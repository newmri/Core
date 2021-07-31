#pragma once

#include "CoreList.h"

template<typename T>
CoreList<T>::CoreList()
{
	this->head = this->tail = GET_INSTANCE(CoreMemoryPoolManager<CoreNode<T>>).Alloc(this->dataDefaultReserveSize);

	if (this->head)
	{
		CoreContainer<T>::SetSize(0);
	}
}

template<typename T>
CoreList<T>::~CoreList()
{

}

template<typename T>
void CoreList<T>::Init(void)
{
	CoreContainer<T>::Init();
}

template<typename T>
CoreList<T>::CoreList(const CoreList<T>& rhs)
{
	Copy(rhs);
}

template<typename T>
CoreList<T>& CoreList<T>::operator=(const CoreList<T>& rhs)
{
	Copy(rhs);
	return *this;
}

template<typename T>
void CoreList<T>::Copy(const CoreList<T>& rhs)
{

}

template<typename T>
void CoreList<T>::clear(void)
{
}

template<typename T>
void CoreList<T>::push_back(const T& data)
{

}

template<typename T>
void CoreList<T>::push_back(T&& data)
{
	CoreNode<T>* newNode = GET_INSTANCE(CoreMemoryPoolManager<CoreNode<T>>).Alloc(this->dataDefaultReserveSize);

	new(&newNode->data) T(std::move(data));

	WRITE_LOCK(this->mutex);

	this->tail->next = newNode;
	this->tail = newNode;

	CoreContainer<T>::SetSize(this->dataSize + 1);
}
