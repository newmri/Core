#pragma once

#include <map>

#include "CoreInclude.h"

/* 지금 구조에서는 포인터 받는 곳에서 배열 처럼 사용 불가
현재 구조: [헤더, 데이터]
          [헤더, 데이터]
		  -> 데이터가 순차적이 아니다.
필요 구조: [헤더], [헤더]
          [데이터], [데이터]
          
*/
template<typename T>
class CoreMemoryPool
{
private:
	typedef struct stBlockInfo
	{
		stBlockInfo()
		{
			offset = 0;
			allocatedNum = 0;
		}

		stBlockInfo(size_t offset) : offset(offset)
		{
			allocatedNum = 0;
		}

		size_t offset;
		size_t allocatedNum;
	}BlockInfo;

public:
	DEFAULT_CONSTRUCTOR(CoreMemoryPool<T>)

	CoreMemoryPool(const size_t maxBlockNum)
	{
		Init(maxBlockNum);
	}

	~CoreMemoryPool()
	{
		free(this->block);
	}

public:
	bool Init(const size_t maxBlockNum)
	{
		if (maxBlockNum > CORE_BYTE_MAX)
			return false;

		this->maxBlockNum = this->remainedBlockNum = maxBlockNum;
		this->blockInfoSize = sizeof(BlockInfo);
		this->blockDataSize = sizeof(T);
		this->blockTotalSize = this->blockInfoSize + this->blockDataSize;

		this->block = (CORE_BYTE_PTR)malloc(this->blockTotalSize * this->maxBlockNum);

		size_t offset = 0;

		for (size_t i = 0; i < this->maxBlockNum; ++i)
		{
			new((this->block + offset)) BlockInfo(offset);
			offset += this->blockInfoSize;
			new((this->block + offset)) T();
			offset += this->blockDataSize;
		}

		return true;
	}

	T* Alloc(const size_t needBlockNum)
	{
		WRITE_LOCK(this->mutex);

		if (!CanAlloc(needBlockNum))
			return nullptr;

		T* data = GetData(this->currOffset);

		SetBlockInfo(this->currOffset);
		SetRemainedBlockNum(this->remainedBlockNum - needBlockNum);

		return data;
	}

	void DeAlloc(T* block) noexcept
	{
		WRITE_LOCK(this->mutex);

		if (IS_SAME(0, CanDeAlloc(block)))
		{
			return;
		}

		BlockInfo* blockInfo = GetBlockInfo(block);

		SetRemainedBlockNum(this->remainedBlockNum + blockInfo->allocatedNum);
		blockInfo->allocatedNum = 0;
		block->~T();
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
			return false;

		if (IS_NULL(block))
			return false;

		return true;
	}


private:

	size_t GetBlockInfoOffset(const size_t offset)
	{
		return offset * this->blockTotalSize;
	}

	BlockInfo* GetBlockInfo(const size_t offset)
	{
		return reinterpret_cast<BlockInfo*>(this->block + GetBlockInfoOffset(offset));
	}

	BlockInfo* GetBlockInfo(T* data)
	{
		CORE_BYTE_PTR rawPtr = reinterpret_cast<CORE_BYTE_PTR>(data);
		rawPtr -= this->blockInfoSize;
		return reinterpret_cast<BlockInfo*>(rawPtr);
	}

	size_t GetDataOffset(const size_t offset)
	{
		return (offset * this->blockTotalSize) + this->blockInfoSize;
	}

	T* GetData(const size_t offset)
	{
		return reinterpret_cast<T*>(this->block + GetDataOffset(offset));
	}

	void SetBlockInfo(const size_t offset, const size_t allocatedNum = 1)
	{
		BlockInfo* blockInfo = GetBlockInfo(offset);

		blockInfo->allocatedNum = allocatedNum;
	}

	void SetRemainedBlockNum(const size_t remainedBlockNum)
	{
		this->remainedBlockNum = remainedBlockNum;
	}

private:
	size_t remainedBlockNum = 0;
	size_t maxBlockNum = 0;
	size_t blockInfoSize = 0;
	size_t blockDataSize = 0;
	size_t blockTotalSize = 0;
private:
	CORE_BYTE_PTR block = nullptr;
	size_t currOffset = 0;
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

