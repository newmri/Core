#pragma once

#define CORE_DEFAULT = default
#define CORE_DELETE = delete
#define CORE_PURE = 0

#define READ_LOCK(MUTEX) std::shared_lock<std::shared_mutex> lock(MUTEX)
#define WRITE_LOCK(MUTEX) std::unique_lock<std::shared_mutex> lock(MUTEX)

#define IS_SAME(A, B) (A == B)
#define IS_NOT_SAME(A, B) (A != B)
#define IS_ZERO(NUMBER) (IS_SAME(0, NUMBER))
#define IS_NULL(PTR) (IS_SAME(nullptr, PTR))
#define IS_NOT_NULL(PTR) (IS_NOT_SAME(nullptr, PTR))

#define TO_STR(NUMBER) (std::to_string(NUMBER))
#define ENUM_TO_STR(ENUM) std::string(#ENUM)

template< typename T >
void SAFE_FREE(T* PTR) { if (PTR) { free(PTR); PTR = nullptr; } }


template< typename T >
void SAFE_DELETE(T* PTR) { if (PTR) { delete PTR; PTR = nullptr; } }

template< typename T >
void SAFE_DELETE_ARRAY(T* PTR) { if (PTR) { delete[] PTR; PTR = nullptr; } }

template< typename T >
void SAFE_DELETE(size_t num, T* PTR)
{
	if (IS_SAME(0, num))
		return;

	if (1 < num)
		SAFE_DELETE_ARRAY(PTR);
	else
		SAFE_DELETE(PTR);
}

#ifdef KNOWN_L1_CACHE_LINE_SIZE
inline constexpr size_t LINE = KNOWN_L1_CACHE_LINE_SIZE;
#else
inline constexpr size_t LINE = std::hardware_destructive_interference_size;
#endif

#define CACHE_ALIGN __declspec(align(LINE))

static constexpr size_t CORE_SMALL_SIZE = 128;
static constexpr size_t CORE_MIDDLE_SIZE = 512;
static constexpr size_t CORE_BIG_SIZE = 1024;

#include "CoreSingletonMacro.h"