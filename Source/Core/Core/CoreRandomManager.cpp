#include "CoreInclude.h"

IMPLEMENT_SINGLETON(CoreRandomManager)

void CoreRandomManager::Init(void)
{

}

void CoreRandomManager::Release(void)
{
	GetInstance().~CoreRandomManager();
}

bool CoreRandomManager::GetRandomByPercent(const size_t percent, const size_t maxPercent)
{
	return GetRandomByPercent(static_cast<float>(percent / maxPercent));
}

bool CoreRandomManager::GetRandomByPercent(const float percent)
{
	static thread_local std::random_device randomDevice;
	static thread_local std::mt19937 generator(randomDevice());
	std::bernoulli_distribution distribution(percent);
	return distribution(generator);
}

int CoreRandomManager::GetRandom(const int from, const int to)
{
	static thread_local std::random_device randomDevice;
	static thread_local std::mt19937 generator(randomDevice());
	std::uniform_int_distribution<int> distribution(from, to);
	return distribution(generator);
}

size_t CoreRandomManager::GetRandom(const size_t from, const size_t to)
{
	static thread_local std::random_device randomDevice;
	static thread_local std::mt19937 generator(randomDevice());
	std::uniform_int_distribution<size_t> distribution(from, to);
	return distribution(generator);
}

size_t CoreRandomManager::GetRandom(const CoreVector<size_t>& percentList)
{
	static thread_local std::random_device randomDevice;
	static thread_local std::mt19937 generator(randomDevice());
	std::discrete_distribution<size_t> distribution{ percentList.cbegin(), percentList.cend() };
	return distribution(generator);
}

float CoreRandomManager::GetRandom(const float from, const float to)
{
	static thread_local std::random_device randomDevice;
	static thread_local std::mt19937 generator(randomDevice());
	std::uniform_real_distribution<float> distribution(from, to);
	return distribution(generator);
}

double CoreRandomManager::GetRandom(const double from, const double to)
{
	static thread_local std::random_device randomDevice;
	static thread_local std::mt19937 generator(randomDevice());
	std::uniform_real_distribution<double> distribution(from, to);
	return distribution(generator);
}