#pragma once

#include "CoreSort.h"

constexpr unsigned int HashCode(const char* const str)
{
	return str[0] ? static_cast<unsigned int>(str[0]) + 0xEDB8832Full * HashCode(str + 1) : 8603;
}