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
	GET_INSTANCE(CoreMemoryPoolManager<CoreNode<T>>).DeAlloc(this->head);
	clear();
}

template<typename T>
void CoreList<T>::Init(void)
{
	CoreContainer<T>::Init();
}

template<typename T>
CoreList<T>::CoreList(CoreList<T>& rhs)
{
	READ_LOCK(rhs.mutex);
	Copy(rhs);
}

template<typename T>
CoreList<T>& CoreList<T>::operator=(CoreList<T>& rhs)
{
	READ_LOCK(rhs.mutex);
	Copy(rhs);
	return *this;
}

template<typename T>
void CoreList<T>::Copy(const CoreList<T>& rhs)
{
	clear();

	WRITE_LOCK(this->mutex);
	this->head = this->tail = GET_INSTANCE(CoreMemoryPoolManager<CoreNode<T>>).Alloc(rhs.dataSize);

	if (this->head)
	{
		const_iterator begin = rhs.cbegin();
		const_iterator end = rhs.cend();

		for(; begin != end; ++begin)
			push_back(CreateNewNode(*begin));

		CoreContainer<T>::SetSize(rhs.dataSize);
		CoreContainer<T>::SetDefaultReserveSize(rhs.dataDefaultReserveSize);
	}
}

template<typename T>
void CoreList<T>::clear(void)
{
	WRITE_LOCK(this->mutex);

	if (IS_NULL(this->head))
		return;

	CoreNode<T>* currNode = this->head->next;

	while (currNode)
	{
		GET_INSTANCE(CoreMemoryPoolManager<CoreNode<T>>).DeAlloc(currNode);
		currNode = currNode->next;
	}

	this->head->next = nullptr;
	this->tail = this->head;
}

template<typename T>
void CoreList<T>::push_front(const T& data)
{
	CoreNode<T>* newNode = CreateNewNode(data);

	WRITE_LOCK(this->mutex);
	push_front(newNode);
}

template<typename T>
void CoreList<T>::push_front(T&& data)
{
	CoreNode<T>* newNode = CreateNewNode(data);

	WRITE_LOCK(this->mutex);
	push_front(newNode);
}

template<typename T>
void CoreList<T>::push_back(const T& data)
{
	CoreNode<T>* newNode = CreateNewNode(data);

	WRITE_LOCK(this->mutex);
	push_back(newNode);
}

template<typename T>
void CoreList<T>::push_back(T&& data)
{
	CoreNode<T>* newNode = CreateNewNode(data);

	WRITE_LOCK(this->mutex);
	push_back(newNode);
}

template<typename T>
CoreNode<T>* CoreList<T>::CreateNewNode(const T& data)
{
	CoreNode<T>* newNode = GET_INSTANCE(CoreMemoryPoolManager<CoreNode<T>>).Alloc(this->dataDefaultReserveSize);
	new(&newNode->data) T(data);
	return newNode;
}

template<typename T>
CoreNode<T>* CoreList<T>::CreateNewNode(T&& data)
{
	CoreNode<T>* newNode = GET_INSTANCE(CoreMemoryPoolManager<CoreNode<T>>).Alloc(this->dataDefaultReserveSize);
	new(&newNode->data) T(std::move(data));
	return newNode;
}

template<typename T>
void CoreList<T>::push_front(CoreNode<T>* newNode)
{
	newNode->next = this->head->next;
	this->head->next = newNode;

	CoreContainer<T>::SetSize(this->dataSize + 1);
}

template<typename T>
void CoreList<T>::push_back(CoreNode<T>* newNode)
{
	this->tail->next = newNode;
	this->tail = newNode;

	CoreContainer<T>::SetSize(this->dataSize + 1);
}
