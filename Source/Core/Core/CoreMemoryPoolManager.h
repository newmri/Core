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
		std::shared_ptr<Node> next;
	};

public:
	template<typename... Types>
	T* Alloc(const size_t maxBlockNum, const size_t needBlockNum, Types... args);

public:
	bool IsValidBlockNum(const size_t maxBlockNum, const size_t needBlockNum);
	void CheckAndAllocHead(const size_t maxBlockNum);

public:
	void DeAlloc(T* body);

public:
	size_t GetPageNum(void);

private:
	size_t pageNum = 0;

private:
	std::shared_ptr<Node> head;
	std::shared_mutex mutex;
};

#include "CoreMemoryPoolManager.hpp"