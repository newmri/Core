#pragma once

#include "CoreContainer.h"
#include "CoreNode.h"

template<typename T>
class CoreQueue : public CoreContainer<T>
{
	OVERRIDE_OBJECT(CoreQueue<T>)

public:
	CoreQueue(CoreQueue<T>& rhs);

public:
	CoreQueue<T>& operator=(CoreQueue<T>& rhs);

private:
	void Copy(const CoreQueue<T>& rhs);

public:
	virtual void clear(void) override;

public:
	void push(const T& data);
	void push(T&& data);
	void pop(void);

public:
	T front(void);
	T back(void);

protected:
	CoreNode<T>* CreateNewNode(const T& data);
	CoreNode<T>* CreateNewNode(T&& data);
	void push(CoreNode<T>* newNode);

public:
	typedef CoreIterator<T> iterator;
	typedef const CoreIterator<T> const_iterator;

public:
	const_iterator cbegin(void) const { return const_iterator(this->head->next); }
	iterator begin(void) { return iterator(this->head->next); }
	const_iterator cend(void) const { return const_iterator(this->tail->next); }
	iterator end(void) { return iterator(this->tail->next); }

protected:
	CoreNode<T>* head = nullptr;
	CoreNode<T>* tail = nullptr;
};

#include "CoreQueue.hpp"