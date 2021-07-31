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
	typedef T* iterator;
	typedef const T* const_iterator;

//public:
//	const_iterator cbegin(void) const { return this->data; }
//	iterator begin(void) { return const_cast<iterator>(static_cast<const CoreVector<T>&>(*this).cbegin()); }
//	const_iterator cend(void) const { return &this->data[this->dataSize]; }
//	iterator end(void) { return const_cast<iterator>(static_cast<const CoreVector<T>&>(*this).cend()); }

private:
	CoreNode<T>* head;
};

#include "CoreList.hpp"