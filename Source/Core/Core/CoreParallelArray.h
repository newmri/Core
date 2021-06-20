#pragma once

#include "CoreParallelSTL.h"

template<typename T>
class CoreParallelArray : public CoreParallelSTL<T>
{
public:
	CoreParallelArray() CORE_DEFAULT;
	CoreParallelArray(const size_t newCapacity);
	virtual ~CoreParallelArray() CORE_DEFAULT;

public:
	virtual void Push(const T newData) override;
	virtual void Pop(void) override;
	virtual void Clear(void) override;
};

#include "CoreParallelArray.hpp"