#pragma once

#include "CoreInclude.h"

struct BlockHeader
{
	size_t allocatedNum = 0;
};

template<typename T, size_t MAX_BLOCK_NUM>
class CoreMemoryPool
{
	struct BlockInfo
	{
		CACHE_ALIGN size_t remainedBlockNum = MAX_BLOCK_NUM;
		static constexpr size_t blockHeaderSize = sizeof(BlockHeader);
		static constexpr size_t blockHeaderTotalSize = blockHeaderSize * MAX_BLOCK_NUM;
		static constexpr size_t blockBodySize = sizeof(T);
		static constexpr size_t blockBodyTotalSize = blockBodySize * MAX_BLOCK_NUM;
		static constexpr size_t blockTotalSize = blockHeaderTotalSize + blockBodyTotalSize;
	};

public:
	CoreMemoryPool();
	~CoreMemoryPool();

public:
	bool Init(void);

	T* Alloc(const size_t needBlockNum);

	bool IsMyBody(T* blockBody);

	void DeAlloc(T* blockBody) noexcept;

public:
	size_t GetRemainedBlockNum(void);
	size_t GetMaxBlockNum(void);

private:
	bool CanAlloc(const size_t needBlockNum, size_t& startIndex, size_t& endIndex);
	bool CanDeAlloc(T* blockBody, size_t& startIndex, size_t& endIndex);

private:
	BlockHeader* GetBlockHeader(const size_t index);
	size_t GetBlockHeaderOffset(const size_t index);
	size_t GetIndex(T* blockBody);
	size_t GetBlockBodyOffset(const size_t index);
	T* GetBlockBody(const size_t index);

private:
	void SetBlock(void);
	void SetBlockHeader(BlockHeader* blockHeader, const size_t allocatedNum, const bool changeRefCnt);
	void SetBlockHeader(const size_t index, const size_t allocatedNum = 1, const bool changeRefCnt = true);

	void SetRemainedBlockNum(const size_t remainedBlockNum);

private:
	BlockInfo blockInfo;
	size_t allocPos = 0;
	CORE_BYTE_PTR block[BlockInfo::blockTotalSize];
	CACHE_ALIGN std::shared_mutex mutex;
};

#include "CoreMemoryPool.hpp"
