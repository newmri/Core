#pragma once

template<typename T, const size_t N>
class CoreArray
{
public:
	CoreArray();
	CoreArray(std::initializer_list<T> list);
	CoreArray(const CoreArray<T, N>& rhs);
	virtual ~CoreArray();

public:
	CoreArray<T, N>& operator=(const T* rhs);
	CoreArray<T, N>& operator=(CoreArray<T, N>& rhs);
	const T& operator[](const size_t index) const;
	T& operator[](const size_t index);

public:
	size_t size(void) const;

private:
	void Copy(const T* rhs);
	void Copy(const CoreArray<T, N>& rhs);

public:
	typedef T* iterator;
	typedef const T* const_iterator;

public:
	const_iterator cbegin(void) const { return this->data; }
	iterator begin(void) { return const_cast<iterator>(static_cast<const CoreArray<T, N>&>(*this).cbegin()); }
	const_iterator cend(void) const { return this->data + N; }
	iterator end(void) { return const_cast<iterator>(static_cast<const CoreArray<T, N>&>(*this).cend()); }

private:
	std::shared_mutex mutex;
	T data[N];
};

#include "CoreArray.hpp"