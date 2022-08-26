#pragma once

namespace CoreUtil
{
	int32_t IntRound(const float_t value);
	int32_t IntCeil(const float_t value);

	template<typename T>
	T Min(const T lValue, const T rValue)
	{
		return (lValue > rValue ? rValue : lValue);
	}

	template<typename T>
	T Max(const T lValue, const T rValue)
	{
		return (lValue > rValue ? lValue : rValue);
	}

	std::string GetThreadID(void);
}