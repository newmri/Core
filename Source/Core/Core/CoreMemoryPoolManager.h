#pragma once

#include "CoreMemoryPool.h"

template<typename T, size_t MAX_BLOCK_NUM>
struct Node
{
	Node()
	{
		page.Init();
	}

	CoreMemoryPool<T, MAX_BLOCK_NUM> page;
	std::unique_ptr<Node<T, MAX_BLOCK_NUM>> next;
};

template<typename T, size_t MAX_BLOCK_NUM>
class CoreMemoryPoolManager
{
	DECLARE_SINGLETON(CoreMemoryPoolManager)

public:
	T* Alloc(const size_t needBlockNum = 1);

private:
	bool IsValidBlockNum(const size_t needBlockNum);

private:
	void CheckAndAllocHead(void);

public:
	void DeAlloc(T* blockBody);

public:
	size_t GetPageNum(void);

private:
	CACHE_ALIGN size_t pageNum = 0;

private:
	std::unique_ptr<Node<T, MAX_BLOCK_NUM>> head;
	CACHE_ALIGN std::shared_mutex mutex;
};

#include "CoreMemoryPoolManager.hpp"