#pragma once

#include "CoreMemoryPoolManager.h"

IMPLEMENT_BLOCK_TEMPLATE_SINGLETON(CoreMemoryPoolManager)

template<typename T, size_t MAX_BLOCK_NUM>
void CoreMemoryPoolManager<T, MAX_BLOCK_NUM>::Init(void)
{
	CheckAndAllocHead();
}

template<typename T, size_t MAX_BLOCK_NUM>
void CoreMemoryPoolManager<T, MAX_BLOCK_NUM>::Release(void)
{
	GetInstance().~CoreMemoryPoolManager();
}

template<typename T, size_t MAX_BLOCK_NUM>
T* CoreMemoryPoolManager<T, MAX_BLOCK_NUM>::Alloc(const size_t needBlockNum)
{
	if (!IsValidBlockNum(needBlockNum))
		return nullptr;

	T* blockBody = nullptr;

	Node<T, MAX_BLOCK_NUM>* currNode = this->head.get();

	do
	{
		blockBody = currNode->page.Alloc(needBlockNum);

		if (IS_NULL(blockBody))
		{
			if (IS_NULL(currNode->next))
			{
				WRITE_LOCK(this->mutex);
				if (IS_NULL(currNode->next))
				{
					currNode->next = std::make_unique<Node<T, MAX_BLOCK_NUM>>();
					++this->pageNum;
				}
			}

			currNode = currNode->next.get();
		}

	} while (IS_NULL(blockBody));


	return blockBody;
}

template<typename T, size_t MAX_BLOCK_NUM>
bool CoreMemoryPoolManager<T, MAX_BLOCK_NUM>::IsValidBlockNum(const size_t needBlockNum)
{
	if (needBlockNum > MAX_BLOCK_NUM)
	{
		std::string errorMessage = "maxBlockNum: " + TO_STR(MAX_BLOCK_NUM) + " needBlockNum: " + TO_STR(needBlockNum);
		CORE_LOG.Log(LogType::LOG_ERROR, errorMessage);
		return false;
	}

	return true;
}

template<typename T, size_t MAX_BLOCK_NUM>
void CoreMemoryPoolManager<T, MAX_BLOCK_NUM>::CheckAndAllocHead(void)
{
	if (IS_NULL(this->head))
	{
		head = std::make_unique<Node<T, MAX_BLOCK_NUM>>();
		++this->pageNum;
	}
}

template<typename T, size_t MAX_BLOCK_NUM>
void CoreMemoryPoolManager<T, MAX_BLOCK_NUM>::DeAlloc(T* blockBody)
{
	Node<T, MAX_BLOCK_NUM>* currNode = this->head.get();

	READ_LOCK(this->mutex);

	do
	{
		if (currNode->page.IsMyBody(blockBody))
		{
			currNode->page.DeAlloc(blockBody);
			break;
		}

		currNode = currNode->next.get();
	} while (currNode);
}

template<typename T, size_t MAX_BLOCK_NUM>
size_t CoreMemoryPoolManager<T, MAX_BLOCK_NUM>::GetPageNum(void)
{
	READ_LOCK(this->mutex);
	return this->pageNum;
}
