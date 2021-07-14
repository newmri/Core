#pragma once

template<typename T, const size_t N>
class CoreArray
{
public:
	CoreArray();
	CoreArray(std::initializer_list<T> list);
	CoreArray(CoreArray<T, N>& rhs);

public:
	CoreArray<T, N>& operator=(const CoreArray<T, N>& rhs);
	T& operator[](const size_t index);
	const T& operator[](const size_t index) const;

public:
	size_t size(void) const;

private:
	void Copy(const CoreArray<T, N>& rhs);

public:
	typedef T* iterator;
	typedef const T* const_iterator;

public:
	iterator begin(void) { return this->data; }
	const_iterator cbegin(void) const { return this->data; }
	iterator end(void) { return this->data + N; };
	const_iterator cend(void) const { return this->data + N; };

private:
	std::shared_mutex mutex;
	T data[N];
};

#include "CoreArray.hpp"