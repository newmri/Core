#pragma once

#include "CoreQueue.h"

template<typename T>
CoreQueue<T>::CoreQueue()
{
	this->head = this->tail = GET_INSTANCE(CoreMemoryPoolManager<CoreNode<T>>).Alloc(this->dataDefaultReserveSize);

	if (this->head)
	{
		CoreContainer<T>::SetSize(0);
	}
}

template<typename T>
CoreQueue<T>::~CoreQueue()
{
	GET_INSTANCE(CoreMemoryPoolManager<CoreNode<T>>).DeAlloc(this->head);
	clear();
}

template<typename T>
void CoreQueue<T>::Init(void)
{
	CoreContainer<T>::Init();
}

template<typename T>
CoreQueue<T>::CoreQueue(CoreQueue<T>& rhs)
{
	READ_LOCK(rhs.mutex);
	Copy(rhs);
}

template<typename T>
CoreQueue<T>& CoreQueue<T>::operator=(CoreQueue<T>& rhs)
{
	READ_LOCK(rhs.mutex);
	Copy(rhs);
	return *this;
}

template<typename T>
void CoreQueue<T>::Copy(const CoreQueue<T>& rhs)
{
	clear();

	WRITE_LOCK(this->mutex);
	this->head = this->tail = GET_INSTANCE(CoreMemoryPoolManager<CoreNode<T>>).Alloc(rhs.dataSize);

	if (this->head)
	{
		const_iterator begin = rhs.cbegin();
		const_iterator end = rhs.cend();

		for (; begin != end; ++begin)
			push_back(CreateNewNode(*begin));

		CoreContainer<T>::SetSize(rhs.dataSize);
		CoreContainer<T>::SetDefaultReserveSize(rhs.dataDefaultReserveSize);
	}
}

template<typename T>
void CoreQueue<T>::clear(void)
{
	WRITE_LOCK(this->mutex);

	if (IS_SAME(this->dataSize, 0))
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
void CoreQueue<T>::push(const T& data)
{
	CoreNode<T>* newNode = CreateNewNode(data);
	push(newNode);
}

template<typename T>
void CoreQueue<T>::push(T&& data)
{
	CoreNode<T>* newNode = CreateNewNode(data);
	push(newNode);
}

template<typename T>
void CoreQueue<T>::pop(void)
{
	CoreNode<T>* deleteNode = nullptr;
	{
		WRITE_LOCK(this->mutex);

		if (IS_SAME(this->dataSize, 0))
			return;

		deleteNode = this->head->next;
		this->head->next = deleteNode->next;
		CoreContainer<T>::SetSize(this->dataSize - 1);
	}

	GET_INSTANCE(CoreMemoryPoolManager<CoreNode<T>>).DeAlloc(deleteNode);
}

template<typename T>
T CoreQueue<T>::front(void)
{
	READ_LOCK(this->mutex);

	if (IS_SAME(this->dataSize, 0))
		return T();

	return this->head->next->data;
}

template<typename T>
T CoreQueue<T>::back(void)
{
	READ_LOCK(this->mutex);

	if (IS_SAME(this->dataSize, 0))
		return T();

	return this->tail->data;
}

template<typename T>
CoreNode<T>* CoreQueue<T>::CreateNewNode(const T& data)
{
	CoreNode<T>* newNode = GET_INSTANCE(CoreMemoryPoolManager<CoreNode<T>>).Alloc(this->dataDefaultReserveSize);
	new(&newNode->data) T(data);
	return newNode;
}

template<typename T>
CoreNode<T>* CoreQueue<T>::CreateNewNode(T&& data)
{
	CoreNode<T>* newNode = GET_INSTANCE(CoreMemoryPoolManager<CoreNode<T>>).Alloc(this->dataDefaultReserveSize);
	new(&newNode->data) T(std::move(data));
	return newNode;
}

template<typename T>
void CoreQueue<T>::push(CoreNode<T>* newNode)
{
	WRITE_LOCK(this->mutex);

	this->tail->next = newNode;
	this->tail = newNode;

	CoreContainer<T>::SetSize(this->dataSize + 1);
}