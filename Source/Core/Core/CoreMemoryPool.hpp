#pragma once

#include "CoreMemoryPool.h"

template<typename T, size_t MAX_BLOCK_NUM>
CoreMemoryPool<T, MAX_BLOCK_NUM>::CoreMemoryPool()
{
	Init();
}

template<typename T, size_t MAX_BLOCK_NUM>
CoreMemoryPool<T, MAX_BLOCK_NUM>::~CoreMemoryPool()
{

}

template<typename T, size_t MAX_BLOCK_NUM>
bool CoreMemoryPool<T, MAX_BLOCK_NUM>::Init(void)
{
	SetBlock();

	return true;
}

template<typename T, size_t MAX_BLOCK_NUM>
T* CoreMemoryPool<T, MAX_BLOCK_NUM>::Alloc(const size_t needBlockNum)
{
	size_t startIndex = 0, endIndex = 0;

	{
		WRITE_LOCK(this->mutex);

		if (IS_SAME(0, CanAlloc(needBlockNum, startIndex, endIndex)))
			return nullptr;


		for (size_t i = startIndex; i <= endIndex; ++i)
			SetBlockHeader(i);

		SetBlockHeader(startIndex, needBlockNum, false);

		this->allocPos = endIndex + 1;
		SetRemainedBlockNum(this->blockInfo.remainedBlockNum - needBlockNum);
	}

	return GetBlockBody(startIndex);
}

template<typename T, size_t MAX_BLOCK_NUM>
bool CoreMemoryPool<T, MAX_BLOCK_NUM>::IsMyBody(T* blockBody)
{
	return (MAX_BLOCK_NUM > GetIndex(blockBody));
}

template<typename T, size_t MAX_BLOCK_NUM>
void CoreMemoryPool<T, MAX_BLOCK_NUM>::DeAlloc(T* blockBody) noexcept
{
	size_t startIndex = 0, endIndex = 0;

	WRITE_LOCK(this->mutex);

	if (IS_SAME(false, CanDeAlloc(blockBody, startIndex, endIndex)))
		return;

	for (size_t i = startIndex; i <= endIndex; ++i)
		SetBlockHeader(i, 0);

	this->allocPos = 0;
	SetRemainedBlockNum(this->blockInfo.remainedBlockNum + (endIndex - startIndex) + 1);
	blockBody = nullptr;

	return;
}

template<typename T, size_t MAX_BLOCK_NUM>
size_t CoreMemoryPool<T, MAX_BLOCK_NUM>::GetRemainedBlockNum(void)
{
	READ_LOCK(this->mutex);
	return this->blockInfo.remainedBlockNum;
}

template<typename T, size_t MAX_BLOCK_NUM>
size_t CoreMemoryPool<T, MAX_BLOCK_NUM>::GetMaxBlockNum(void)
{
	return MAX_BLOCK_NUM;
}

template<typename T, size_t MAX_BLOCK_NUM>
bool CoreMemoryPool<T, MAX_BLOCK_NUM>::CanAlloc(const size_t needBlockNum, size_t& startIndex, size_t& endIndex)
{
	if (needBlockNum > this->blockInfo.remainedBlockNum)
		return false;

	size_t availableNum = 0;

	for (size_t i = this->allocPos; i < MAX_BLOCK_NUM; ++i)
	{
		BlockHeader* blockHeader = GetBlockHeader(i);

		if (IS_SAME(0, blockHeader->allocatedNum))
			++availableNum;

		else
			availableNum = 0;

		if (availableNum == needBlockNum)
		{
			startIndex = (i + 1) - needBlockNum;
			endIndex = i;
			return true;
		}
	}

	return false;
}

template<typename T, size_t MAX_BLOCK_NUM>
bool CoreMemoryPool<T, MAX_BLOCK_NUM>::CanDeAlloc(T* blockBody, size_t& startIndex, size_t& endIndex)
{
	if (IS_NULL(blockBody))
		return false;

	if (IS_SAME(this->blockInfo.remainedBlockNum, MAX_BLOCK_NUM))
		return false;

	startIndex = GetIndex(blockBody);

	BlockHeader* blockHeader = GetBlockHeader(startIndex);

	if (IS_SAME(0, blockHeader->allocatedNum))
		return false;

	endIndex = startIndex + (blockHeader->allocatedNum - 1);

	return true;
}


template<typename T, size_t MAX_BLOCK_NUM>
BlockHeader* CoreMemoryPool<T, MAX_BLOCK_NUM>::GetBlockHeader(const size_t index)
{
	return reinterpret_cast<BlockHeader*>(this->block + GetBlockHeaderOffset(index));
}

template<typename T, size_t MAX_BLOCK_NUM>
size_t CoreMemoryPool<T, MAX_BLOCK_NUM>::GetBlockHeaderOffset(const size_t index)
{
	return (this->blockInfo.blockHeaderSize * index);
}

template<typename T, size_t MAX_BLOCK_NUM>
size_t CoreMemoryPool<T, MAX_BLOCK_NUM>::GetIndex(T* blockBody)
{
	size_t offset = reinterpret_cast<size_t>(blockBody) - reinterpret_cast<size_t>(GetBlockBody(0));
	offset /= sizeof(void*);
	return offset / this->blockInfo.blockBodySize;
}

template<typename T, size_t MAX_BLOCK_NUM>
size_t CoreMemoryPool<T, MAX_BLOCK_NUM>::GetBlockBodyOffset(const size_t index)
{
	return (this->blockInfo.blockHeaderTotalSize + (index * this->blockInfo.blockBodySize));
}

template<typename T, size_t MAX_BLOCK_NUM>
T* CoreMemoryPool<T, MAX_BLOCK_NUM>::GetBlockBody(const size_t index)
{
	return reinterpret_cast<T*>(this->block + GetBlockBodyOffset(index));
}

template<typename T, size_t MAX_BLOCK_NUM>
void CoreMemoryPool<T, MAX_BLOCK_NUM>::SetBlock(void)
{
	for (size_t i = 0; i < MAX_BLOCK_NUM; ++i)
		new(GetBlockHeader(i)) BlockHeader();
}

template<typename T, size_t MAX_BLOCK_NUM>
void CoreMemoryPool<T, MAX_BLOCK_NUM>::SetBlockHeader(BlockHeader* blockHeader, const size_t allocatedNum, const bool changeRefCnt)
{
	blockHeader->allocatedNum = allocatedNum;
}

template<typename T, size_t MAX_BLOCK_NUM>
void CoreMemoryPool<T, MAX_BLOCK_NUM>::SetBlockHeader(const size_t index, const size_t allocatedNum, const bool changeRefCnt)
{
	BlockHeader* blockHeader = GetBlockHeader(index);
	SetBlockHeader(blockHeader, allocatedNum, changeRefCnt);
}

template<typename T, size_t MAX_BLOCK_NUM>
void CoreMemoryPool<T, MAX_BLOCK_NUM>::SetRemainedBlockNum(const size_t remainedBlockNum)
{
	this->blockInfo.remainedBlockNum = remainedBlockNum;
}