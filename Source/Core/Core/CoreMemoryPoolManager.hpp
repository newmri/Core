#pragma once

#include "CoreMemoryPoolManager.h"

template<typename T>
size_t CoreMemoryPoolManager<T>::pageNum = 0;

template<typename T>
std::unique_ptr<Node<T>> CoreMemoryPoolManager<T>::head;

template<typename T>
std::shared_mutex CoreMemoryPoolManager<T>::mutex;

template<typename T>
T* CoreMemoryPoolManager<T>::Alloc(const size_t maxBlockNum, const size_t needBlockNum)
{
	if (!IsValidBlockNum(maxBlockNum, needBlockNum))
		return nullptr;

	T* blockBody = nullptr;

	WRITE_LOCK(mutex);

	CheckAndAllocHead(maxBlockNum);

	Node<T>* currNode = head.get();

	do
	{
		blockBody = currNode->page.Alloc(needBlockNum);

		if (IS_NULL(blockBody))
		{
			if (IS_NULL(currNode->next))
			{
				currNode->next = std::make_unique<Node<T>>(maxBlockNum);
				++pageNum;
			}

			currNode = currNode->next.get();
		}

	} while (IS_NULL(blockBody));

	return blockBody;
}

template<typename T>
T*& CoreMemoryPoolManager<T>::Share(T*& blockBody)
{
	Node<T>* currNode = head.get();
	CORE_BYTE_PTR byteBody = reinterpret_cast<CORE_BYTE_PTR>(blockBody);

	WRITE_LOCK(mutex);

	while (currNode)
	{
		if (currNode->page.IsMyBody(byteBody))
		{
			currNode->page.Share(blockBody);
			return blockBody;
		}

		currNode = currNode->next.get();
	}

	T* ret = nullptr;
	return ret;
}

template<typename T>
bool CoreMemoryPoolManager<T>::IsValidBlockNum(const size_t maxBlockNum, const size_t needBlockNum)
{
	if (needBlockNum > maxBlockNum)
	{
		std::string errorMessage = "maxBlockNum: " + TO_STR(maxBlockNum) + " needBlockNum: " + TO_STR(needBlockNum);
		CORE_LOG.Log(LogType::LOG_ERROR, errorMessage);
		return false;
	}

	return true;
}

template<typename T>
void CoreMemoryPoolManager<T>::CheckAndAllocHead(const size_t maxBlockNum)
{
	if (IS_NULL(head))
	{
		head = std::make_unique<Node<T>>(maxBlockNum);
		++pageNum;
	}
}

template<typename T>
void CoreMemoryPoolManager<T>::DeAlloc(T*& blockBody)
{
	Node<T>* currNode = head.get();
	CORE_BYTE_PTR byteBody = reinterpret_cast<CORE_BYTE_PTR>(blockBody);

	WRITE_LOCK(mutex);

	while (currNode)
	{
		if (currNode->page.IsMyBody(byteBody))
		{
			currNode->page.DeAlloc(blockBody);
			break;
		}

		currNode = currNode->next.get();
	}
}

template<typename T>
size_t CoreMemoryPoolManager<T>::GetPageNum(void)
{
	READ_LOCK(mutex);
	return pageNum;
}

template<typename T>
bool CoreMemoryPoolManager<T>::IsValid(T*& blockBody)
{
	Node<T>* currNode = head.get();
	CORE_BYTE_PTR byteBody = reinterpret_cast<CORE_BYTE_PTR>(blockBody);

	READ_LOCK(mutex);

	while (currNode)
	{
		if (currNode->page.IsMyBody(byteBody))
			return currNode->page.IsValid(blockBody);

		currNode = currNode->next.get();
	}

	return false;
}