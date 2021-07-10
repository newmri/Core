#pragma once

#define CORE_DEFAULT = default
#define CORE_DELETE = delete
#define CORE_PURE = 0

#define CORE_OUT(TYPE) TYPE&
#define CORE_CONST_PTR(TYPE) const TYPE* const
#define CORE_REF(TYPE) const TYPE&

#define READ_LOCK(MUTEX) std::shared_lock<std::shared_mutex> lock(MUTEX)
#define WRITE_LOCK(MUTEX) std::unique_lock<std::shared_mutex> lock(MUTEX)

#define IS_SAME(A, B) (A == B)
#define IS_NOT_SAME(A, B) (A != B)
#define IS_ZERO(NUMBER) (IS_SAME(0, NUMBER))
#define IS_NULL(PTR) (IS_SAME(nullptr, PTR))
#define IS_NOT_NULL(PTR) (IS_NOT_SAME(nullptr, PTR))

#include "CoreSingletonMacro.h"