#pragma once

#include "CoreContainer.h"

template<typename T>
class CoreVector : public CoreContainer<T>
{
	OVERRIDE_OBJ(CoreVector<T>)

public:
	template<typename... Types>
	CoreVector(const size_t maxBlockNum, const size_t needBlockNum, Types... args);

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
	void push_back(const T data);
	void pop_back(void);

private:
	void SetCapacity(const size_t newCapacity);

private:
	size_t dataCapacity = 0;
};

#include "CoreVector.hpp"