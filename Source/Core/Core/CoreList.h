#pragma once

#include "CoreContainer.h"
#include "CoreNode.h"

template<typename T>
class CoreList : public CoreContainer<T>
{
	OVERRIDE_OBJ(CoreList<T>)

public:
	CoreList(const CoreList<T>& rhs);

public:
	CoreList<T>& operator=(const CoreList<T>& rhs);

private:
	void Copy(const CoreList<T>& rhs);

public:
	virtual void clear(void) override;

public:
	void push_back(const T& data);
	void push_back(T&& data);

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
	size_t dataDefaultReserveSize = 10;
};

#include "CoreList.hpp"