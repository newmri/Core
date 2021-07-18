#pragma once

#include "CoreMemoryPoolManager.h"

IMPLEMENT_TEMPLATE_SINGLETON(CoreMemoryPoolManager)

template<typename T>
void CoreMemoryPoolManager<T>::Init(void)
{
}

template<typename T>
template<typename... Types>
T* CoreMemoryPoolManager<T>::Alloc(const size_t maxBlockNum, const size_t needBlockNum, const bool needCallCtor, Types... args)
{
	if (!IsValidBlockNum(maxBlockNum, needBlockNum))
		return nullptr;

	CheckAndAllocHead(maxBlockNum);

	std::shared_ptr<Node> currNode = this->head;
	T* blockBody = nullptr;

	do
	{
		blockBody = currNode->page.Alloc(needBlockNum, needCallCtor, args...);

		if (IS_NULL(blockBody))
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

	} while (IS_NULL(blockBody));

	return blockBody;
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
	WRITE_LOCK(this->mutex);

	if (IS_NULL(this->head))
	{
		this->head = std::make_shared<Node>(maxBlockNum);
		++pageNum;
	}
}

template<typename T>
void CoreMemoryPoolManager<T>::DeAlloc(void* block, const bool needCallDtor)
{
	DeAlloc(reinterpret_cast<T*>(block), needCallDtor);
}

template<typename T>
void CoreMemoryPoolManager<T>::DeAlloc(T* blockBody, const bool needCallDtor)
{
	std::shared_ptr<Node> currNode = this->head;
	CORE_BYTE_PTR byteBody = reinterpret_cast<CORE_BYTE_PTR>(blockBody);

	while (currNode)
	{
		if (currNode->page.IsMyBody(byteBody))
		{
			currNode->page.DeAlloc(blockBody, needCallDtor);
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