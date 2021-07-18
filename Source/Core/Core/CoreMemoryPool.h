#pragma once

#include "CoreInclude.h"

struct BlockInfo
{
	size_t remainedBlockNum = 0;
	size_t maxBlockNum = 0;
	size_t blockHeaderSize = 0;
	size_t blockHeaderTotalSize = 0;
	size_t blockBodySize = 0;
	size_t blockBodyTotalSize = 0;
	size_t blockTotalSize = 0;
};

struct BlockHeader
{
	size_t allocatedNum = 0;
};

template<typename T>
class CoreMemoryPool
{
public:
	DEFAULT_CONSTRUCTOR(CoreMemoryPool<T>)

	CoreMemoryPool(const size_t maxBlockNum);
	~CoreMemoryPool();

public:
	bool Init(const size_t maxBlockNum);

	template<typename... Types>
	T* Alloc(const size_t needBlockNum, const bool needCallCtor = true, Types... args);

	bool IsMyBody(CORE_BYTE_PTR blockBody);

	void DeAlloc(T* blockBody, const bool needCallDtor = true) noexcept;

public:
	size_t GetRemainedBlockNum(void);
	size_t GetMaxBlockNum(void);

private:
	bool CanAlloc(const size_t needBlockNum, CORE_OUT(size_t) startIndex, CORE_OUT(size_t) endIndex);
	bool CanDeAlloc(T* blockBody, CORE_OUT(size_t) startIndex, CORE_OUT(size_t) endIndex);

private:
	size_t GetBlockHeaderOffset(const size_t index);
	BlockHeader* GetBlockHeader(const size_t index);
	size_t GetIndex(T* blockBody);
	size_t GetBlockBodyOffset(const size_t index);
	T* GetBlockBody(const size_t index);

private:
	void SetInfo(const size_t maxBlockNum);
	void SetBlock(void);
	void SetBlockHeader(BlockHeader* blockHeader, const size_t allocatedNum = 1);
	void SetBlockHeader(const size_t index, const size_t allocatedNum = 1);
	void SetRemainedBlockNum(const size_t remainedBlockNum);

private:
	BlockInfo blockInfo;
	CORE_BYTE_PTR block = nullptr;

private:
	std::shared_mutex mutex;
};

#include "CoreMemoryPool.hpp"

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

