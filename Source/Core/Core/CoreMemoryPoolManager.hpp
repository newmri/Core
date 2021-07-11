#pragma once

#include "CoreMemoryPoolManager.h"

IMPLEMENT_TEMPLATE_SINGLETON(CoreMemoryPoolManager)

template<typename T>
void CoreMemoryPoolManager<T>::Init(void)
{
}

template<typename T>
template<typename... Types>
T* CoreMemoryPoolManager<T>::Alloc(const size_t maxBlockNum, const size_t needBlockNum, Types... args)
{
	if (needBlockNum > maxBlockNum)
	{
		std::string errorMessage = "maxBlockNum: " + std::to_string(maxBlockNum) + " needBlockNum: " + std::to_string(needBlockNum);
		CORE_LOG.Log(LogType::LOG_ERROR, errorMessage);
		return nullptr;
	}

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