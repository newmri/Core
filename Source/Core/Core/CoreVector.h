#pragma once

#include "CoreContainer.h"

template<typename T>
class CoreVector : public CoreContainer<T>
{
public:
	CoreVector();
	template<typename... Types>
	CoreVector(const size_t maxBlockNum, Types... args);
	CoreVector(CoreVector<T>& rhs);
	virtual ~CoreVector() override;

public:
	CoreVector<T>& operator=(CoreVector<T>& rhs);
	const T& operator[](const size_t index) const;
	T& operator[](const size_t index);

private:
	void Copy(const CoreVector<T>& rhs);

public:
	void clear(void);

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
	const_iterator cbegin(void) const { return this->data; }
	iterator begin(void) { return const_cast<iterator>(static_cast<const CoreVector<T>&>(*this).cbegin()); }
	const_iterator cend(void) const { return &this->data[this->dataSize]; }
	iterator end(void) { return const_cast<iterator>(static_cast<const CoreVector<T>&>(*this).cend()); }

private:
	T* data = nullptr;
	size_t dataDefaultReserveSize = 5;
	size_t dataCapacity = 0;
	size_t dataCapacityIncrease = 2;
};

#include "CoreVector.hpp"