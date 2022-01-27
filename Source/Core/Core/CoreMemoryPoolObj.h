#pragma once

template<typename T, size_t MAX_BLOCK_NUM>
class CoreMemoryPoolObj
{
public:
	template <typename... Types>
	static void* operator new(size_t size, Types... args);
	template <typename... Types>
	static void* operator new[](size_t size, Types... args);
	static void operator delete(void* blockBody);
	static void operator delete[](void* blockBody);

public:
	size_t GetPageNum(void);
};

#include "CoreMemoryPoolObj.hpp"