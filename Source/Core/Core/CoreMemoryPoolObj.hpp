#include "CoreMemoryPoolObj.h"
#include "CoreMemoryPoolManager.h"

template<typename T, size_t MAX_BLOCK_NUM>
template<typename... Types>
void* CoreMemoryPoolObj<T, MAX_BLOCK_NUM>::operator new(size_t size, Types... args)
{
	return CoreMemoryPoolManager<T, MAX_BLOCK_NUM>::GetInstance().Alloc();
}

template<typename T, size_t MAX_BLOCK_NUM>
template<typename... Types>
void* CoreMemoryPoolObj<T, MAX_BLOCK_NUM>::operator new[](size_t size, Types... args)
{
	return CoreMemoryPoolManager<T, MAX_BLOCK_NUM>::GetInstance().Alloc((size - sizeof(void*)) / sizeof(T));
}

template<typename T, size_t MAX_BLOCK_NUM>
void CoreMemoryPoolObj<T, MAX_BLOCK_NUM>::operator delete(void* blockBody)
{
	CoreMemoryPoolManager<T, MAX_BLOCK_NUM>::GetInstance().DeAlloc((T*)blockBody);
}

template<typename T, size_t MAX_BLOCK_NUM>
void CoreMemoryPoolObj<T, MAX_BLOCK_NUM>::operator delete[](void* blockBody)
{
	CoreMemoryPoolManager<T, MAX_BLOCK_NUM>::GetInstance().DeAlloc((T*)blockBody);
}

template<typename T, size_t MAX_BLOCK_NUM>
size_t CoreMemoryPoolObj<T, MAX_BLOCK_NUM>::GetPageNum(void)
{
	return CoreMemoryPoolManager<T, MAX_BLOCK_NUM>::GetInstance().GetPageNum();
}