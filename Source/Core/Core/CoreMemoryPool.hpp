#pragma once

#include "CoreMemoryPool.h"

template<typename T>
CoreMemoryPool<T>::CoreMemoryPool(const size_t maxBlockNum)
{
	Init(maxBlockNum);
}

template<typename T>
CoreMemoryPool<T>::~CoreMemoryPool()
{
	free(this->block);
}

template<typename T>
bool CoreMemoryPool<T>::Init(const size_t maxBlockNum)
{
	SetInfo(maxBlockNum);
	SetBlock();

	return true;
}

template<typename T>
template<typename... Types>
T* CoreMemoryPool<T>::Alloc(const size_t needBlockNum, const bool needCallCtor, Types... args)
{
	size_t startIndex = 0, endIndex = 0;

	WRITE_LOCK(this->mutex);

	if (IS_SAME(0, CanAlloc(needBlockNum, startIndex, endIndex)))
		return nullptr;

	for (size_t i = startIndex; i <= endIndex; ++i)
	{
		SetBlockHeader(i);
		if (needCallCtor)
		{
			new(GetBlockBody(i)) T(args...);
		}
	}

	SetBlockHeader(startIndex, needBlockNum);

	T* blockBody = GetBlockBody(startIndex);

	SetRemainedBlockNum(this->blockInfo.remainedBlockNum - needBlockNum);

	return blockBody;
}

template<typename T>
bool CoreMemoryPool<T>::IsMyBody(CORE_BYTE_PTR blockBody)
{
	return ((this->block < blockBody) && ((this->block + GetBlockBodyOffset(this->blockInfo.maxBlockNum - 1)) >= blockBody));
}

template<typename T>
void CoreMemoryPool<T>::DeAlloc(T* blockBody, const bool needCallDtor) noexcept
{
	size_t startIndex = 0, endIndex = 0;

	WRITE_LOCK(this->mutex);

	if (IS_SAME(false, CanDeAlloc(blockBody, startIndex, endIndex)))
		return;

	for (size_t i = startIndex; i <= endIndex; ++i)
	{
		SetBlockHeader(i, 0);
		if (needCallDtor)
		{
			GetBlockBody(i)->~T();
		}
	}

	SetRemainedBlockNum(this->blockInfo.remainedBlockNum + (endIndex - startIndex) + 1);

	return;
}

template<typename T>
void CoreMemoryPool<T>::DeAllocAll(void) noexcept
{
	SAFE_FREE(this->block);
}

template<typename T>
size_t CoreMemoryPool<T>::GetRemainedBlockNum(void)
{
	READ_LOCK(this->mutex);
	return this->blockInfo.remainedBlockNum;
}

template<typename T>
size_t CoreMemoryPool<T>::GetMaxBlockNum(void)
{
	READ_LOCK(this->mutex);
	return this->blockInfo.maxBlockNum;
}

template<typename T>
bool CoreMemoryPool<T>::CanAlloc(const size_t needBlockNum, CORE_OUT(size_t) startIndex, CORE_OUT(size_t) endIndex)
{
	if (needBlockNum > this->blockInfo.remainedBlockNum)
		return false;

	size_t availableNum = 0;

	for (size_t i = 0; i < this->blockInfo.maxBlockNum; ++i)
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

template<typename T>
bool CoreMemoryPool<T>::CanDeAlloc(T* blockBody, CORE_OUT(size_t) startIndex, CORE_OUT(size_t) endIndex)
{
	if (IS_NULL(blockBody))
		return false;

	if (IS_SAME(this->blockInfo.remainedBlockNum, this->blockInfo.maxBlockNum))
		return false;

	startIndex = GetIndex(blockBody);

	BlockHeader* blockHeader = GetBlockHeader(startIndex);

	if (IS_SAME(0, blockHeader->allocatedNum))
		return false;

	endIndex = startIndex + (blockHeader->allocatedNum - 1);

	return true;
}

template<typename T>
size_t CoreMemoryPool<T>::GetBlockHeaderOffset(const size_t index)
{
	return index * this->blockInfo.blockHeaderSize;
}

template<typename T>
BlockHeader* CoreMemoryPool<T>::GetBlockHeader(const size_t index)
{
	return reinterpret_cast<BlockHeader*>(this->block + GetBlockHeaderOffset(index));
}

template<typename T>
size_t CoreMemoryPool<T>::GetIndex(T* blockBody)
{
	size_t offset = reinterpret_cast<size_t>(blockBody) - reinterpret_cast<size_t>(GetBlockHeader(this->blockInfo.maxBlockNum));
	return offset / this->blockInfo.blockBodySize;
}

template<typename T>
size_t CoreMemoryPool<T>::GetBlockBodyOffset(const size_t index)
{
	return (this->blockInfo.blockHeaderTotalSize + (index * this->blockInfo.blockBodySize));
}

template<typename T>
T* CoreMemoryPool<T>::GetBlockBody(const size_t index)
{
	return reinterpret_cast<T*>(this->block + GetBlockBodyOffset(index));
}

template<typename T>
void CoreMemoryPool<T>::SetInfo(const size_t maxBlockNum)
{
	this->blockInfo.maxBlockNum = this->blockInfo.remainedBlockNum = maxBlockNum;

	this->blockInfo.blockHeaderSize = sizeof(BlockHeader);
	this->blockInfo.blockHeaderTotalSize = this->blockInfo.blockHeaderSize * this->blockInfo.maxBlockNum;

	this->blockInfo.blockBodySize = sizeof(T);
	this->blockInfo.blockBodyTotalSize = this->blockInfo.blockBodySize * this->blockInfo.maxBlockNum;

	this->blockInfo.blockTotalSize = this->blockInfo.blockHeaderTotalSize + this->blockInfo.blockBodyTotalSize;
}

template<typename T>
void CoreMemoryPool<T>::SetBlock(void)
{
	this->block = (CORE_BYTE_PTR)malloc(this->blockInfo.blockTotalSize);

	for (size_t i = 0; i < this->blockInfo.maxBlockNum; ++i)
		new(GetBlockHeader(i)) BlockHeader();
}

template<typename T>
void CoreMemoryPool<T>::SetBlockHeader(BlockHeader* blockHeader, const size_t allocatedNum)
{
	blockHeader->allocatedNum = allocatedNum;
}

template<typename T>
void CoreMemoryPool<T>::SetBlockHeader(const size_t index, const size_t allocatedNum)
{
	BlockHeader* blockHeader = GetBlockHeader(index);

	blockHeader->allocatedNum = allocatedNum;
}

template<typename T>
void CoreMemoryPool<T>::SetRemainedBlockNum(const size_t remainedBlockNum)
{
	this->blockInfo.remainedBlockNum = remainedBlockNum;
}