#include "CoreInclude.h"

CoreMemoryPool::~CoreMemoryPool()
{
	if (IS_ZERO(this->blockSize))
		return;

	free(this->page);
}

bool CoreMemoryPool::Init(const size_t blockSize, const size_t maxBlockNum)
{
	if (this->maxBlockNum > CORE_BYTE_MAX)
		return false;

	this->blockSize = blockSize;
	this->maxBlockNum = maxBlockNum;

	this->page = static_cast<CORE_BYTE_PTR>(malloc(this->blockSize * this->maxBlockNum));

	this->remainedBlockNum = this->maxBlockNum;

	CORE_BYTE_PTR p = this->page;

	for (CORE_INT i = 0; i < this->maxBlockNum; ++i)
	{
		*p = i + 1;
		p += this->blockSize;
	}

	return true;
}

void* CoreMemoryPool::Alloc(const size_t size)
{
	WRITE_LOCK(this->mutex);

	if (!CanAlloc(size))
		return nullptr;

	CORE_BYTE_PTR block = this->page + (this->currBlock * size);

	SetCurrBlock(*block);

	size_t blockNum = GetBlockNumBySize(size);

	allocatedBlockNum[block] = blockNum;
	SetRemainedBlockNum(this->remainedBlockNum - blockNum);

	return static_cast<void*>(block);
}

void CoreMemoryPool::DeAlloc(void* block) noexcept
{
	CORE_BYTE_PTR blockPTR = static_cast<CORE_BYTE_PTR>(block);

	WRITE_LOCK(this->mutex);

	size_t allocatedBlockNum = CanDeAlloc(blockPTR);

	if (IS_SAME(0, allocatedBlockNum))
		return;

	*(static_cast<CORE_INT_PTR>(block)) = static_cast<CORE_INT>(this->currBlock);

	CORE_INT newBlock = static_cast<CORE_INT>((blockPTR - this->page) / static_cast<CORE_INT>(this->blockSize));

	SetCurrBlock(newBlock);
	SetRemainedBlockNum(this->remainedBlockNum + allocatedBlockNum);
	allocatedBlockNum[blockPTR] = 0;
}

size_t CoreMemoryPool::GetRemainedBlockNum(void)
{
	READ_LOCK(this->mutex);
	return this->remainedBlockNum;
}

size_t CoreMemoryPool::GetMaxBlockNum(void)
{
	READ_LOCK(this->mutex);
	return this->maxBlockNum;
}

size_t CoreMemoryPool::GetBlockNumBySize(const size_t size) const
{
	return (size / this->blockSize);
}

bool CoreMemoryPool::CanAlloc(const size_t size)
{
	size_t needBlockNum = GetBlockNumBySize(size);

	return (needBlockNum <= this->remainedBlockNum);
}

size_t CoreMemoryPool::CanDeAlloc(CORE_BYTE_PTR block)
{
	if (IS_SAME(this->remainedBlockNum, this->maxBlockNum))
		return 0;

	if (IS_NULL(block))
		return 0;

	auto iter = allocatedBlockNum.find(block);

	if (IS_SAME(iter, allocatedBlockNum.end()))
		return 0;

	return iter->second;
}

void CoreMemoryPool::SetRemainedBlockNum(const size_t remainedBlockNum)
{
	this->remainedBlockNum = remainedBlockNum;
}

void CoreMemoryPool::SetCurrBlock(CORE_INT newBlock)
{
	this->currBlock = newBlock;
}
