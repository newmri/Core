#pragma once

#include "CoreMemoryPool.h"

template<typename T>
class CoreMemoryPoolManager
{
	DECLARE_SINGLETON(CoreMemoryPoolManager<T>)

public:
	struct Node
	{
		Node() CORE_DEFAULT;

		Node(const size_t maxBlockNum)
		{
			page.Init(maxBlockNum);
		}

		CoreMemoryPool<T> page;
		std::unique_ptr<Node> next;
	};

public:
	template<typename... Types>
	T* Alloc(const size_t maxBlockNum, const size_t needBlockNum = 1, const bool needCallCtor = true, Types... args);

	// EX
	//int* value = GET_INSTANCE(CoreMemoryPoolManager<int>).Alloc(1, 1);
	//int*& value2 = GET_INSTANCE(CoreMemoryPoolManager<int>).Share(value);

	T*& Share(T*& blockBody);

public:
	bool IsValidBlockNum(const size_t maxBlockNum, const size_t needBlockNum);

private:
	void CheckAndAllocHead(const size_t maxBlockNum);

public:
	void DeAlloc(T*& blockBody, const bool needCallDtor = true);

public:
	size_t GetPageNum(void);

private:
	CACHE_ALIGN size_t pageNum = 0;

private:
	std::unique_ptr<Node> head;
	CACHE_ALIGN std::shared_mutex mutex;
};

#include "CoreMemoryPoolManager.hpp"