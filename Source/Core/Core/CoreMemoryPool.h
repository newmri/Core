#pragma once

#include <map>

#include "CoreDataType.h"

class CoreMemoryPool
{
public:
	~CoreMemoryPool();

public:
	bool Init(const size_t blockSize, const size_t maxBlockNum);
	void* Alloc(const size_t size);
	void DeAlloc(void* block) noexcept;

public:
	size_t GetRemainedBlockNum(void);
	size_t GetMaxBlockNum(void);

private:
	size_t GetBlockNumBySize(const size_t size) const;
	bool CanAlloc(const size_t size);
	size_t CanDeAlloc(CORE_BYTE_PTR block);

private:
	void SetRemainedBlockNum(const size_t remainedBlockNum);
	void SetCurrBlock(CORE_INT newBlock);

private:
	size_t remainedBlockNum = 0;
	size_t currBlock = 0;
	size_t blockSize = 0;
	size_t maxBlockNum = 0;
	CORE_BYTE_PTR page = nullptr;

private:
	std::shared_mutex mutex;
	std::map<CORE_BYTE_PTR, size_t> allocatedBlockNum;
};

#define DECLARE_MEMORY_POOL(CLASS)																					\
	public:																											\
	static bool InitMemoryPool(const size_t maxBlockNum = CORE_BYTE_MAX);											\
	static size_t GetMemoryPoolRemainedBlockNum(void);																\
																													\
	static void* operator new(const size_t size)																	\
	{																												\
		return instance->Alloc(size);																				\
	}																												\
																													\
	static void* operator new[](const size_t size)																	\
	{																												\
		return instance->Alloc(size);																				\
	}																												\
																													\
	static void operator delete(void* block) noexcept																\
	{																												\
		instance->DeAlloc(block);																					\
	}																												\
																													\
	static void operator delete[](void* block) noexcept																\
	{																												\
		instance->DeAlloc(block);																					\
	}																												\
																													\
    private:																										\
    static std::unique_ptr<CoreMemoryPool> instance;																\
    static std::once_flag onceFlag;

#define IMPLEMENAT_MEMORY_POOL(CLASS)																				\
	std::unique_ptr<CoreMemoryPool> CLASS::instance;																\
	std::once_flag CLASS::onceFlag;																					\
	bool CLASS::InitMemoryPool(const size_t maxBlockNum)															\
	{																												\
		bool ret = false;																							\
		call_once(CLASS::onceFlag, [&]()																			\
		{																											\
			CLASS::instance = std::make_unique<CoreMemoryPool>();													\
			ret = (*(CLASS::instance.get())).Init(sizeof(CLASS), maxBlockNum);										\
		});																											\
		return ret;																									\
	};																												\
																													\
	size_t CLASS::GetMemoryPoolRemainedBlockNum(void)																\
	{																												\
		return (*(CLASS::instance.get())).GetRemainedBlockNum();													\
	}
