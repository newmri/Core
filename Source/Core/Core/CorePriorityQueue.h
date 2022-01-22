#pragma once

#include "CoreContainer.h"
#include "CoreNode.h"

template<typename T>
class CorePriorityQueue : public CoreContainer<T>
{
public:
	CorePriorityQueue(bool (*compare)(const T&, const T&) = CoreAscendingOrder);
	virtual ~CorePriorityQueue() override;
	CorePriorityQueue(CorePriorityQueue<T>& rhs);

protected:																
	virtual void Init(void) override;

public:
	CorePriorityQueue<T>& operator=(CorePriorityQueue<T>& rhs);

private:
	void Copy(const CorePriorityQueue<T>& rhs);

public:
	virtual void clear(void) override;

public:
	void push(const T& data);
	void push(T&& data);
	void pop(void);

public:
	T top(void);

private:
	CoreNode<T>* CreateNewNode(const T& data);
	CoreNode<T>* CreateNewNode(T&& data);
	void push(CoreNode<T>* newNode);

public:
	typedef CoreIterator<T> iterator;
	typedef const CoreIterator<T> const_iterator;

public:
	const_iterator cbegin(void) const { return const_iterator(this->head->next); }
	iterator begin(void) { return iterator(this->head->next); }
	const_iterator cend(void) const { return const_iterator(nullptr); }
	iterator end(void) { return iterator(nullptr); }

private:
	CoreNode<T>* head = nullptr;
	bool (*compare)(const T&, const T&);
};

#include "CorePriorityQueue.hpp"