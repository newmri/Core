#pragma once

#include "CoreParallelSTL.h"

template<typename T>
class CoreParallelArray : public CoreParallelSTL<T>
{
public:
	CoreParallelArray() CORE_DEFAULT;
	CoreParallelArray(const size_t size);
	virtual ~CoreParallelArray();

public:
	virtual void Clear(void) override;

protected:
	T* data = nullptr;
};

#include "CoreParallelArray.hpp"