#pragma once

#include "CoreQueue.h"

template<typename T>
CoreQueue<T>::CoreQueue()
{
	Init();
}

template<typename T>
CoreQueue<T>::~CoreQueue()
{
	clear();

	SAFE_DELETE(this->head);
}

template<typename T>
void CoreQueue<T>::Init(void)
{
	CoreContainer<T>::Init();

	this->head = new CoreNode<T>();
	this->tail = this->head;
	this->head->next = this->tail;

	if (this->head)
	{
		CoreContainer<T>::SetSize(0);
	}
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

	SAFE_DELETE(this->head);
	Init();

	if (this->head)
	{
		const_iterator begin = rhs.cbegin();
		const_iterator end = rhs.cend();

		for (; begin != end; ++begin)
			push(CreateNewNode(*begin));

		CoreContainer<T>::SetSize(rhs.dataSize);
	}
}

template<typename T>
void CoreQueue<T>::clear(void)
{
	CoreNode<T>* deleteNode = nullptr;

	WRITE_LOCK(this->mutex);

	if (IS_SAME(this->dataSize, 0))
		return;

	CoreNode<T>* currNode = this->head->next;

	while (IS_NOT_NULL(currNode))
	{
		deleteNode = currNode;
		currNode = currNode->next;
		SAFE_DELETE(deleteNode);
	}

	this->tail = this->head;
	this->head->next = this->tail;
	CoreContainer<T>::SetSize(0);
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

		CoreContainer<T>::SetSize(this->dataSize - 1);

		if (IS_SAME(this->dataSize, 0))
			this->head->next = this->tail;
		else
			this->head->next = deleteNode->next;
	}

	SAFE_DELETE(deleteNode);
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
	CoreNode<T>* newNode = new CoreNode<T>(data);
	return newNode;
}

template<typename T>
CoreNode<T>* CoreQueue<T>::CreateNewNode(T&& data)
{
	CoreNode<T>* newNode = new CoreNode<T>(std::move(data));
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
