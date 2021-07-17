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
	virtual void Clear(void) override;

public:
	size_t capacity(void);
	void reserve(const size_t newCapacity);

private:
	size_t dataCapacity = 0;
};

#include "CoreVector.hpp"