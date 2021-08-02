#pragma once

#include "CoreContainer.h"
#include "CoreNode.h"

template<typename T>
class CoreList : public CoreContainer<T>
{
	OVERRIDE_OBJ(CoreList<T>)

public:
	CoreList(CoreList<T>& rhs);

public:
	CoreList<T>& operator=(CoreList<T>& rhs);

private:
	void Copy(const CoreList<T>& rhs);

public:
	virtual void clear(void) override;

public:
	void push_front(const T& data);
	void push_front(T&& data);
	void push_back(const T& data);
	void push_back(T&& data);

private:
	CoreNode<T>* CreateNewNode(const T& data);
	CoreNode<T>* CreateNewNode(T&& data);
	void push_front(CoreNode<T>* newNode);
	void push_back(CoreNode<T>* newNode);

public:
	typedef CoreIterator<T> iterator;
	typedef const CoreIterator<T> const_iterator;

public:
	const_iterator cbegin(void) const { return const_iterator(this->head->next); }
	iterator begin(void) { return iterator(this->head->next); }
	const_iterator cend(void) const { return const_iterator(this->tail->next); }
	iterator end(void) { return iterator(this->tail->next); }

private:
	CoreNode<T>* head = nullptr;
	CoreNode<T>* tail = nullptr;
};

#include "CoreList.hpp"