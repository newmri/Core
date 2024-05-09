#pragma once

#include "CorePriorityQueue.h"


template<typename T>
CorePriorityQueue<T>::CorePriorityQueue(bool (*compare)(const T&, const T&)) : compare(compare)
{
	this->head = new CoreNode<T>();

	if (this->head)
	{
		CoreContainer<T>::SetSize(0);
	}
}

template<typename T>
CorePriorityQueue<T>::~CorePriorityQueue()
{
	clear();
	SAFE_DELETE(this->head);
}

template<typename T>
CorePriorityQueue<T>::CorePriorityQueue(CorePriorityQueue<T>& rhs)
{
	READ_LOCK(rhs.mutex);
	Copy(rhs);
}

template<typename T>
CorePriorityQueue<T>& CorePriorityQueue<T>::operator=(CorePriorityQueue<T>& rhs)
{
	READ_LOCK(rhs.mutex);
	Copy(rhs);
	return *this;
}

template<typename T>
void CorePriorityQueue<T>::Copy(const CorePriorityQueue<T>& rhs)
{
	clear();

	WRITE_LOCK(this->mutex);
	this->head = new CoreNode<T>();

	if (this->head)
	{
		const_iterator begin = rhs.cbegin();
		const_iterator end = rhs.cend();

		for (; begin != end; ++begin)
			push(CreateNewNode(*begin));

		CoreContainer<T>::SetSize(rhs.dataSize);
		CoreContainer<T>::SetDefaultReserveSize(rhs.dataDefaultReserveSize);
	}

	this->compare = rhs.compare;
}

template<typename T>
void CorePriorityQueue<T>::clear(void)
{
	CoreNode<T>* deleteNode = nullptr;

	WRITE_LOCK(this->mutex);

	if (IS_SAME(this->dataSize, 0))
		return;

	CoreNode<T>* currNode = this->head->next;

	while (currNode)
	{
		deleteNode = currNode;
		currNode = currNode->next;
		SAFE_DELETE(deleteNode);
	}

	this->head->next = nullptr;
	CoreContainer<T>::SetSize(0);
}

template<typename T>
void CorePriorityQueue<T>::push(const T& data)
{
	CoreNode<T>* newNode = CreateNewNode(data);
	push(newNode);
}

template<typename T>
void CorePriorityQueue<T>::push(T&& data)
{
	CoreNode<T>* newNode = CreateNewNode(data);
	push(newNode);
}

template<typename T>
void CorePriorityQueue<T>::pop(void)
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

	SAFE_DELETE(deleteNode);
}

template<typename T>
T* CorePriorityQueue<T>::top(void)
{
	READ_LOCK(this->mutex);

	if (IS_SAME(this->dataSize, 0))
		return nullptr;

	return &this->head->next->data;
}

template<typename T>
CoreNode<T>* CorePriorityQueue<T>::CreateNewNode(const T& data)
{
	CoreNode<T>* newNode = new CoreNode<T>(data);
	return newNode;
}

template<typename T>
CoreNode<T>* CorePriorityQueue<T>::CreateNewNode(T&& data)
{
	CoreNode<T>* newNode = new CoreNode<T>(std::move(data));
	return newNode;
}

template<typename T>
void CorePriorityQueue<T>::push(CoreNode<T>* newNode)
{
	WRITE_LOCK(this->mutex);

	if (IS_SAME(this->dataSize, 0))
	{
		this->head->next = newNode;
		CoreContainer<T>::SetSize(this->dataSize + 1);
		return;
	}

	CoreNode<T>* prevNode = this->head;
	CoreNode<T>* currNode = prevNode->next;
	CoreNode<T>* nextNode = nullptr;

	while (currNode)
	{
		nextNode = currNode->next;

		if (IS_SAME(currNode->data, newNode->data))
		{
			newNode->next = currNode->next;
			currNode->next = newNode;
			break;
		}

		else if (compare(currNode->data, newNode->data))
		{
			prevNode->next = newNode;
			newNode->next = currNode;
			break;
		}

		else if (IS_NULL(nextNode))
		{
			currNode->next = newNode;
			break;
		}

		prevNode = currNode;
		currNode = nextNode;
	}

	CoreContainer<T>::SetSize(this->dataSize + 1);
}
