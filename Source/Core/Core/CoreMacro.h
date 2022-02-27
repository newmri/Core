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
#define TO_WSTR(NUMBER) (std::to_wstring(NUMBER))
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

#define SAFE_DELETE_DTOR(ROWS, RAW_PTR, DATA_TYPE, DATA_PTR)				\
	if(RAW_PTR)																\
	{																		\
		if(1 < ROWS)														\
		{																	\
			for (size_t i = 0; i < ROWS; ++i)								\
				DATA_PTR[i].~DATA_TYPE();									\
																			\
			delete[] RAW_PTR;												\
		}																	\
		else																\
		{																	\
			DATA_PTR->~DATA_TYPE();											\
			delete RAW_PTR;													\
		}																	\
		RAW_PTR = nullptr;													\
		DATA_PTR = nullptr;													\
	}

#define CSV_LOAD_AND_TO_HAS_MAP(FILE_PATH, IN_TYPE, OUT, KEY)				\
	{																		\
		char* table = nullptr;												\
		size_t rows = CSV_LOAD.Load(FILE_PATH, table);						\
		RAW_DATA_TO_HASH_MAP(rows, table, IN_TYPE, OUT, KEY);				\
	}

#define RAW_DATA_TO_HASH_MAP(ROWS, RAW_PTR, IN_TYPE, OUT, KEY)				\
	IN_TYPE* data = reinterpret_cast<IN_TYPE*>(RAW_PTR);					\
																			\
	for (size_t i = 0; i < ROWS; ++i)										\
	{																		\
		IN_TYPE info = data[i];												\
		OUT[data[i].KEY] = std::make_shared<IN_TYPE>(info);					\
	}																		\
																			\
	SAFE_DELETE_DTOR(ROWS, RAW_PTR, IN_TYPE, data);

#define CSV_LOAD_ONE_ROW(FILE_PATH, IN_TYPE, OUT)							\
	{																		\
		char* table = nullptr;												\
		size_t rows = CSV_LOAD.Load(FILE_PATH, table);						\
		IN_TYPE* data = reinterpret_cast<IN_TYPE*>(table);					\
		OUT = std::make_shared<IN_TYPE>(data[0]);							\
		SAFE_DELETE_DTOR(rows, table, IN_TYPE, data);						\
	}

#define CSV_LOAD_AND_TO_VECTOR(FILE_PATH, IN_TYPE, OUT)						\
	{																		\
		char* table = nullptr;												\
		size_t rows = CSV_LOAD.Load(FILE_PATH, table);						\
		IN_TYPE* data = reinterpret_cast<IN_TYPE*>(table);					\
																			\
		for (size_t i = 0; i < rows; ++i)									\
		{																	\
			IN_TYPE info = data[i];											\
			OUT.push_back(info);											\
		}																	\
	}

#ifdef _UNICODE
#define WIDEN(x)        L ## x
#define WIDEN2(x)       WIDEN(x)
#define __WFILE__       WIDEN2(__FILE__)
#define __WFUNCTION__	WIDEN2(__FUNCTION__)
#else
#define __WFILE__          __FILE__
#define __WFUNCTION__ __FUNCTION__
#endif

#include "CoreSingletonMacro.h"