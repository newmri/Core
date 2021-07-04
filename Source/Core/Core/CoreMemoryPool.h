#pragma once

#include <map>

#include "CoreInclude.h"

template<typename T>
class CoreMemoryPool
{
private:
	typedef struct stBlockInfo
	{
		stBlockInfo()
		{
			allocatedNum = 0;
		}

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

		SetInfo(maxBlockNum);
		SetBlock();

		return true;
	}

	T* Alloc(const size_t needBlockNum)
	{
		WRITE_LOCK(this->mutex);

		size_t startIndex = 0, endIndex = 0;

		if (!CanAlloc(needBlockNum, startIndex, endIndex))
			return nullptr;

		SetBlockInfo(startIndex, needBlockNum);

		for (size_t i = startIndex + 1; i <= endIndex; ++i)
		{
			SetBlockInfo(i);
		}

		T* data = GetData(startIndex);

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

	bool CanAlloc(const size_t needBlockNum, CORE_OUT(size_t) startIndex, CORE_OUT(size_t) endIndex)
	{
		if (needBlockNum > this->remainedBlockNum)
			return false;

		size_t availableNum = 0;

		for (size_t i = 0; i < this->maxBlockNum; ++i)
		{
			BlockInfo* blockInfo = GetBlockInfo(i);

			if (IS_SAME(0, blockInfo->allocatedNum))
			{
				++availableNum;
			}

			else
			{
				availableNum = 0;
			}

			if (availableNum == needBlockNum)
			{
				startIndex = (needBlockNum - 1) - i;
				endIndex = i;
				return true;
			}
		}

		return false;
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

	size_t GetBlockInfoOffset(const size_t index)
	{
		return index * this->blockInfoSize;
	}

	BlockInfo* GetBlockInfo(const size_t index)
	{
		return reinterpret_cast<BlockInfo*>(this->block + GetBlockInfoOffset(index));
	}

	BlockInfo* GetBlockInfo(T* data)
	{
		CORE_BYTE_PTR rawPtr = reinterpret_cast<CORE_BYTE_PTR>(data);
		rawPtr -= this->blockInfoSize;
		return reinterpret_cast<BlockInfo*>(rawPtr);
	}

	size_t GetDataOffset(const size_t index)
	{
		return (this->blockInfoTotalSize + (index * this->blockDataSize));
	}

	T* GetData(const size_t index)
	{
		return reinterpret_cast<T*>(this->block + GetDataOffset(index));
	}

	void SetInfo(const size_t maxBlockNum)
	{
		this->maxBlockNum = this->remainedBlockNum = maxBlockNum;

		this->blockInfoSize = sizeof(BlockInfo);
		this->blockInfoTotalSize = this->blockInfoSize * this->maxBlockNum;

		this->blockDataSize = sizeof(T);
		this->blockTotalSize = this->blockInfoSize + this->blockDataSize;
	}

	void SetBlock(void)
	{
		this->block = (CORE_BYTE_PTR)malloc(this->blockTotalSize * this->maxBlockNum);

		size_t offset = 0;

		for (size_t i = 0; i < this->maxBlockNum; ++i)
		{
			new((this->block + offset)) BlockInfo();
			offset += this->blockInfoSize;
		}

		for (size_t i = 0; i < this->maxBlockNum; ++i)
		{
			new((this->block + offset)) T();
			offset += this->blockDataSize;
		}
	}

	void SetBlockInfo(BlockInfo* blockInfo, const size_t allocatedNum = 1)
	{
		blockInfo->allocatedNum = allocatedNum;
	}

	void SetBlockInfo(const size_t index, const size_t allocatedNum = 1)
	{
		BlockInfo* blockInfo = GetBlockInfo(index);

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
	size_t blockInfoTotalSize = 0;
	size_t blockDataSize = 0;
	size_t blockTotalSize = 0;

private:
	CORE_BYTE_PTR block = nullptr;

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

