#pragma once

#include <map>

#include "CoreInclude.h"

template<typename T>
class CoreMemoryPool
{
private:
	typedef struct stBlockHeader
	{
		stBlockHeader()
		{
			allocatedNum = 0;
		}

		size_t allocatedNum;
	}BlockHeader;

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
		SetInfo(maxBlockNum);
		SetBlock();

		return true;
	}

	T* Alloc(const size_t needBlockNum)
	{
		size_t startIndex = 0, endIndex = 0;

		WRITE_LOCK(this->mutex);

		if (IS_SAME(0, CanAlloc(needBlockNum, startIndex, endIndex)))
			return nullptr;

		SetBlockHeader(startIndex, needBlockNum);

		for (size_t i = startIndex + 1; i <= endIndex; ++i)
		{
			SetBlockHeader(i);
		}

		T* data = GetBody(startIndex);
		
		SetRemainedBlockNum(this->remainedBlockNum - needBlockNum);

		return data;
	}

	void DeAlloc(T* block) noexcept
	{
		size_t startIndex = 0, endIndex = 0;

		WRITE_LOCK(this->mutex);

		if (IS_SAME(0, CanDeAlloc(block, startIndex, endIndex)))
		{
			return;
		}

		for (size_t i = startIndex; i <= endIndex; ++i)
		{
			SetBlockHeader(i, 0);
			GetBody(i)->~T();
		}

		SetRemainedBlockNum(this->remainedBlockNum + (endIndex - startIndex) + 1);
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
			BlockHeader* blockHeader = GetBlockHeader(i);

			if (IS_SAME(0, blockHeader->allocatedNum))
			{
				++availableNum;
			}

			else
			{
				availableNum = 0;
			}

			if (availableNum == needBlockNum)
			{
				startIndex = (i + 1) - needBlockNum;
				endIndex = i;
				return true;
			}
		}

		return false;
	}

	size_t CanDeAlloc(T* block, CORE_OUT(size_t) startIndex, CORE_OUT(size_t) endIndex)
	{
		if (IS_NULL(block))
			return false;

		if (IS_SAME(this->remainedBlockNum, this->maxBlockNum))
			return false;

		startIndex = GetIndex(block);

		BlockHeader* blockHeader = GetBlockHeader(startIndex);

		if (IS_SAME(0, blockHeader->allocatedNum))
			return false;

		endIndex = startIndex + (blockHeader->allocatedNum - 1);

		return true;
	}

private:

	size_t GetBlockHeaderOffset(const size_t index)
	{
		return index * this->blockHeaderSize;
	}

	BlockHeader* GetBlockHeader(const size_t index)
	{
		return reinterpret_cast<BlockHeader*>(this->block + GetBlockHeaderOffset(index));
	}

	size_t GetIndex(T* data)
	{
		size_t offset = reinterpret_cast<size_t>(data) - reinterpret_cast<size_t>(GetBlockHeader(this->maxBlockNum));
		return offset / blockBodySize;
	}

	size_t GetBodyOffset(const size_t index)
	{
		return (this->blockHeaderTotalSize + (index * this->blockBodySize));
	}

	T* GetBody(const size_t index)
	{
		return reinterpret_cast<T*>(this->block + GetBodyOffset(index));
	}

	void SetInfo(const size_t maxBlockNum)
	{
		this->maxBlockNum = this->remainedBlockNum = maxBlockNum;

		this->blockHeaderSize = sizeof(BlockHeader);
		this->blockHeaderTotalSize = this->blockHeaderSize * this->maxBlockNum;

		this->blockBodySize = sizeof(T);
		this->blockBodyTotalSize = this->blockBodySize * this->maxBlockNum;

		this->blockTotalSize = this->blockHeaderTotalSize + this->blockBodyTotalSize;
	}

	void SetBlock(void)
	{
		this->block = (CORE_BYTE_PTR)malloc(this->blockTotalSize);

		size_t offset = 0;

		for (size_t i = 0; i < this->maxBlockNum; ++i)
		{
			new((this->block + offset)) BlockHeader();
			offset += this->blockHeaderSize;
		}

		for (size_t i = 0; i < this->maxBlockNum; ++i)
		{
			new((this->block + offset)) T();
			offset += this->blockBodySize;
		}
	}

	void SetBlockHeader(BlockHeader* blockHeader, const size_t allocatedNum = 1)
	{
		blockHeader->allocatedNum = allocatedNum;
	}

	void SetBlockHeader(const size_t index, const size_t allocatedNum = 1)
	{
		BlockHeader* blockHeader = GetBlockHeader(index);

		blockHeader->allocatedNum = allocatedNum;
	}

	void SetRemainedBlockNum(const size_t remainedBlockNum)
	{
		this->remainedBlockNum = remainedBlockNum;
	}

private:
	size_t remainedBlockNum = 0;
	size_t maxBlockNum = 0;
	size_t blockHeaderSize = 0;
	size_t blockHeaderTotalSize = 0;
	size_t blockBodySize = 0;
	size_t blockBodyTotalSize = 0;
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

