#pragma once

template<typename T>
class CoreParallelSTL
{
public:
	CoreParallelSTL() CORE_DEFAULT;
	CoreParallelSTL(const size_t newCapacity);
	virtual ~CoreParallelSTL() CORE_DEFAULT;

public:
	virtual void Push(const T newData) CORE_PURE;

protected:
	virtual void Push(void);

public:
	virtual void Pop(void);
	virtual void Clear(void);

public:
	size_t GetCapacity(void);
	size_t GetSize(void);

private:
	void SetSize(const size_t newSize);
	void SetCapacity(const size_t newCapacity);

protected:
	size_t capacity = 0;
	size_t size = 0;

protected:
	std::shared_mutex mutex;
};

#include "CoreParallelSTL.h"