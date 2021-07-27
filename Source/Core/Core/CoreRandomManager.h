#pragma once

#include <random>

#define CORE_RANDOM_MANAGER GET_INSTANCE(CoreRandomManager)

class CoreRandomManager
{
	DECLARE_SINGLETON(CoreRandomManager)

public:
	bool GetRandomByPercent(const size_t percent, const size_t maxPercent);
	bool GetRandomByPercent(const float percent = 0.5f);
	int GetRandom(const int from, const int to);
	size_t GetRandom(const size_t from, const size_t to);
	size_t GetRandom(const CoreVector<size_t>& percentList);
	float GetRandom(const float from, const float to);
	double GetRandom(const double from, const double to);
};
