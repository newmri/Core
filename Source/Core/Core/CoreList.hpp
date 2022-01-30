#pragma once

#include "CoreList.h"

template<typename T>
CoreList<T>::CoreList()
{
	Init();
}

template<typename T>
CoreList<T>::~CoreList()
{
	clear();

	SAFE_DELETE(this->head);
}

template<typename T>
void CoreList<T>::Init(void)
{
	
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
	SAFE_DELETE(this->head);
	CoreQueue<T>::Init();
	Init();

	if (this->head)
	{
		const_iterator begin = rhs.cbegin();
		const_iterator end = rhs.cend();

		for (; begin != end; ++begin)
			push_back(CreateNewNode(*begin));

		CoreContainer<T>::SetSize(rhs.dataSize);
	}
}

template<typename T>
void CoreList<T>::clear(void)
{
	CoreQueue<T>::clear();
}

template<typename T>
void CoreList<T>::push_front(const T& data)
{
	CoreNode<T>* newNode = CoreQueue<T>::CreateNewNode(data);
	push_front(newNode);
}

template<typename T>
void CoreList<T>::push_front(T&& data)
{
	CoreNode<T>* newNode = CoreQueue<T>::CreateNewNode(data);
	push_front(newNode);
}

template<typename T>
void CoreList<T>::push_back(const T& data)
{
	CoreNode<T>* newNode = CoreQueue<T>::CreateNewNode(data);
	CoreQueue<T>::push(newNode);
}

template<typename T>
void CoreList<T>::push_back(T&& data)
{
	CoreNode<T>* newNode = CoreQueue<T>::CreateNewNode(data);
	CoreQueue<T>::push(newNode);
}

template<typename T>
void CoreList<T>::remove(const T& data)
{
	return remove_if([&](const T& other) { return data == other; });
}

template<typename T>
template<typename FUNC>
void CoreList<T>::remove_if(FUNC Compare)
{
	size_t decreaseNum = 0;

	WRITE_LOCK(this->mutex);

	if (IS_SAME(this->dataSize, 0))
		return;

	CoreNode<T>* currNode = this->head;

	while (IS_NOT_NULL(currNode) && IS_NOT_NULL(currNode->next))
	{
		if (Compare(currNode->next->data))
		{
			SAFE_DELETE(currNode->next);

			currNode->next = currNode->next->next;

			if (IS_NULL(currNode->next))
				this->tail = currNode;

			++decreaseNum;
		}

		currNode = currNode->next;
	}

	CoreContainer<T>::SetSize(this->dataSize - decreaseNum);
}

template<typename T>
void CoreList<T>::push_front(CoreNode<T>* newNode)
{
	WRITE_LOCK(this->mutex);

	newNode->next = this->head->next;
	this->head->next = newNode;

	CoreContainer<T>::SetSize(this->dataSize + 1);
}
