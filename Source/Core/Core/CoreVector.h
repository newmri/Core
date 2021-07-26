#pragma once

#include "CoreContainer.h"

template<typename T>
class CoreVector : public CoreContainer<T>
{
	OVERRIDE_OBJ(CoreVector<T>)

public:
	template<typename... Types>
	CoreVector(const size_t maxBlockNum, Types... args);
	CoreVector(const CoreVector<T>& rhs);

public:
	CoreVector<T>& operator=(const CoreVector<T>& rhs);
	CORE_OUT(T) operator[](const size_t index);
	CORE_REF(T) operator[](const size_t index) const;

private:
	void Copy(const CoreVector<T>& rhs);

public:
	virtual void clear(void) override;

public:
	size_t capacity(void);
	void reserve(const size_t newCapacity);
	void push_back(const T& data);
	void push_back(T&& data);
	void pop_back(void);

private:
	void SetCapacity(const size_t newCapacity);
	void CheckCapacityAndReAlloc(void);
	T* ReAlloc(void);

public:
	typedef T* iterator;
	typedef const T* const_iterator;

public:
	iterator begin(void) { return this->data; }
	const_iterator cbegin(void) const { return begin(); }
	iterator end(void) { return &this->data[this->dataSize]; }
	const_iterator cend(void) const { return end(); }

private:
	size_t dataCapacity = 0;
	size_t dataDefaultReserveSize = 5;
	size_t dataCapacityIncrease = 2;
};

#include "CoreVector.hpp"