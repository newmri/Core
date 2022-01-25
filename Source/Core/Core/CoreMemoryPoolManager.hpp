#pragma once

#include "CoreMemoryPoolManager.h"

IMPLEMENT_TEMPLATE_SINGLETON(CoreMemoryPoolManager)

template<typename T>
void CoreMemoryPoolManager<T>::Init(void)
{
}

template<typename T>
void CoreMemoryPoolManager<T>::Release(void)
{
	GetInstance().~CoreMemoryPoolManager();
}

template<typename T>
template<typename... Types>
T* CoreMemoryPoolManager<T>::Alloc(const size_t maxBlockNum, const size_t needBlockNum, const bool needCallCtor, Types... args)
{
	if (!IsValidBlockNum(maxBlockNum, needBlockNum))
		return nullptr;

	T* blockBody = nullptr;

	WRITE_LOCK(this->mutex);

	CheckAndAllocHead(maxBlockNum);

	Node* currNode = this->head.get();

	do
	{
		blockBody = currNode->page.Alloc(needBlockNum, needCallCtor, args...);

		if (IS_NULL(blockBody))
		{
			if (IS_NULL(currNode->next))
			{
				currNode->next = std::make_unique<Node>(maxBlockNum);
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
	Node* currNode = this->head.get();
	CORE_BYTE_PTR byteBody = reinterpret_cast<CORE_BYTE_PTR>(blockBody);

	WRITE_LOCK(this->mutex);

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
	if (IS_NULL(this->head))
	{
		this->head = std::make_unique<Node>(maxBlockNum);
		++pageNum;
	}
}

template<typename T>
void CoreMemoryPoolManager<T>::DeAlloc(T*& blockBody, const bool needCallDtor)
{
	Node* currNode = this->head.get();
	CORE_BYTE_PTR byteBody = reinterpret_cast<CORE_BYTE_PTR>(blockBody);

	WRITE_LOCK(this->mutex);

	while (currNode)
	{
		if (currNode->page.IsMyBody(byteBody))
		{
			currNode->page.DeAlloc(blockBody, needCallDtor);
			break;
		}

		currNode = currNode->next.get();
	}
}

template<typename T>
size_t CoreMemoryPoolManager<T>::GetPageNum(void)
{
	READ_LOCK(this->mutex);
	return this->pageNum;
}