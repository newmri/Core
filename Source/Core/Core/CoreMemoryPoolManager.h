#pragma once

#include "CoreMemoryPool.h"

template<typename T>
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

template<typename T>
class CoreMemoryPoolManager
{
public:
	template <typename... Types>
	static void* operator new(size_t size, Types... args)
	{
		return Alloc(1, 1);
	}

	template <typename... Types>
	static void* operator new[](size_t size, Types... args)
	{
		return Alloc(1, 1);
	}

	static void operator delete(void* p)
	{
		DeAlloc((T*&)p);
	}

	static void operator delete[](void* p)
	{
		DeAlloc((T*&)p);
	}

private:
	static T* Alloc(const size_t maxBlockNum, const size_t needBlockNum = 1);

	// EX
	//int* value = GET_INSTANCE(CoreMemoryPoolManager<int>).Alloc(1, 1);
	//int*& value2 = GET_INSTANCE(CoreMemoryPoolManager<int>).Share(value);

	static T*& Share(T*& blockBody);

private:
	static bool IsValidBlockNum(const size_t maxBlockNum, const size_t needBlockNum);

private:
	static void CheckAndAllocHead(const size_t maxBlockNum);

private:
	static void DeAlloc(T*& blockBody);

public:
	static size_t GetPageNum(void);

public:
	static bool IsValid(T*& blockBody);

private:
	CACHE_ALIGN static size_t pageNum;

private:
	static std::unique_ptr<Node<T>> head;
	CACHE_ALIGN static std::shared_mutex mutex;
};

#include "CoreMemoryPoolManager.hpp"