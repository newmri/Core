#pragma once

#include <map>

#include "CoreInclude.h"

/*
struct Block
{
	bool used;
	bool allocatedNum;
	T* data;
}

size_t currIndex;
T*의 주소값을 해시 키로 사용, Block의 인덱스에 바로 접근
*/
template<typename T>
class CoreMemoryPool
{
public:
	DEFAULT_CONSTRUCTOR(CoreMemoryPool<T>)

	CoreMemoryPool(const size_t maxBlockNum)
	{
		Init(maxBlockNum);
	}

	~CoreMemoryPool()
	{

	}

public:
	bool Init(const size_t maxBlockNum)
	{
		if (maxBlockNum > CORE_BYTE_MAX)
			return false;

		this->maxBlockNum = maxBlockNum;
		this->remainedBlockNum = this->maxBlockNum;

		return true;
	}

	T* Alloc(const size_t needBlockNum)
	{
		WRITE_LOCK(this->mutex);

		if (!CanAlloc(needBlockNum))
			return nullptr;

		SetRemainedBlockNum(this->remainedBlockNum - needBlockNum);

		return nullptr;
	}

	void DeAlloc(T* block) noexcept
	{
		WRITE_LOCK(this->mutex);

		size_t allocatedBlockNum = CanDeAlloc(block);

		if (IS_SAME(0, allocatedBlockNum))
		{
			return;
		}

		SetRemainedBlockNum(this->remainedBlockNum + allocatedBlockNum);
	}

public:
	size_t GetRemainedBlockNum(void)
	{
		READ_LOCK(this->mutex);
		return this->remainedBlockNum;
	}

	size_t GetMaxBlockNum(void)
	{
		READ_LOCK(this->mutex);
		return this->maxBlockNum;
	}

private:

	bool CanAlloc(const size_t needBlockNum)
	{
		return (needBlockNum <= this->remainedBlockNum);
	}

	size_t CanDeAlloc(T* block)
	{
		if (IS_SAME(this->remainedBlockNum, this->maxBlockNum))
			return 0;

		if (IS_NULL(block))
			return 0;

		return 1;
	}

private:
	void SetRemainedBlockNum(const size_t remainedBlockNum)
	{
		this->remainedBlockNum = remainedBlockNum;
	}

private:
	size_t remainedBlockNum = 0;
	size_t maxBlockNum = 0;

private:

private:
	std::shared_mutex mutex;
};

//#define DECLARE_MEMORY_POOL(CLASS)																					\
//	public:																											\
//	static bool InitMemoryPool(const size_t maxBlockNum = CORE_BYTE_MAX);											\
//	static size_t GetMemoryPoolRemainedBlockNum(void);																\
//																													\
//	static void* operator new(const size_t size)																	\
//	{																												\
//		return instance->Alloc(size);																				\
//	}																												\
//																													\
//	static void* operator new[](const size_t size)																	\
//	{																												\
//		return instance->Alloc(size);																				\
//	}																												\
//																													\
//	static void operator delete(void* block) noexcept																\
//	{																												\
//		instance->DeAlloc(block);																					\
//	}																												\
//																													\
//	static void operator delete[](void* block) noexcept																\
//	{																												\
//		instance->DeAlloc(block);																					\
//	}																												\
//																													\
//    private:																										\
//    static std::unique_ptr<CoreMemoryPool> instance;																\
//    static std::once_flag onceFlag;
//
//#define IMPLEMENAT_MEMORY_POOL(CLASS)																				\
//	std::unique_ptr<CoreMemoryPool> CLASS::instance;																\
//	std::once_flag CLASS::onceFlag;																					\
//	bool CLASS::InitMemoryPool(const size_t maxBlockNum)															\
//	{																												\
//		bool ret = false;																							\
//		call_once(CLASS::onceFlag, [&]()																			\
//		{																											\
//			CLASS::instance = std::make_unique<CoreMemoryPool>();													\
//			ret = (*(CLASS::instance.get())).Init(sizeof(CLASS), maxBlockNum);										\
//		});																											\
//		return ret;																									\
//	};																												\
//																													\
//	size_t CLASS::GetMemoryPoolRemainedBlockNum(void)																\
//	{																												\
//		return (*(CLASS::instance.get())).GetRemainedBlockNum();													\
//	}

