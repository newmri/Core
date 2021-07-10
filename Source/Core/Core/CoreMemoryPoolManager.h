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
	void DeAlloc(T* body);

public:
	size_t GetPageNum(void);

private:
	size_t pageNum = 0;

private:
	std::shared_ptr<Node> head;
	std::shared_mutex mutex;
};

IMPLEMENT_TEMPLATE_SINGLETON(CoreMemoryPoolManager)

template<typename T>
void CoreMemoryPoolManager<T>::Init(void)
{
}

template<typename T>
template<typename... Types>
T* CoreMemoryPoolManager<T>::Alloc(const size_t maxBlockNum, const size_t needBlockNum, Types... args)
{
	{
		WRITE_LOCK(this->mutex);

		if (IS_NULL(this->head))
		{
			this->head = std::make_shared<Node>(maxBlockNum);
			++pageNum;
		}
	}

	std::shared_ptr<Node> currNode = this->head;
	T* body = nullptr;

	do
	{
		body = currNode->page.Alloc(needBlockNum, args...);

		if (IS_NULL(body))
		{
			WRITE_LOCK(this->mutex);

			if (IS_NULL(currNode->next))
			{
				std::shared_ptr<Node> newNode = std::make_shared<Node>(maxBlockNum);
				currNode->next = newNode;
				++pageNum;
			}

			currNode = currNode->next;
		}
	} while (IS_NULL(body));

	return body;
}

template<typename T>
void CoreMemoryPoolManager<T>::DeAlloc(T* body)
{
	std::shared_ptr<Node> currNode = this->head;
	CORE_BYTE_PTR byteBody = reinterpret_cast<CORE_BYTE_PTR>(body);

	while (currNode)
	{
		if (currNode->page.IsMyBody(byteBody))
		{
			currNode->page.DeAlloc(body);
			break;
		}

		currNode = currNode->next;
	}
}

template<typename T>
size_t CoreMemoryPoolManager<T>::GetPageNum(void)
{
	READ_LOCK(this->mutex);
	return this->pageNum;
}