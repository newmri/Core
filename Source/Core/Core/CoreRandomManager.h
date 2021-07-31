#pragma once

#include <random>

#define CORE_RANDOM_MANAGER_INT (GET_INSTANCE(CoreRandomManager<int>))
#define CORE_RANDOM_MANAGER_SIZE_T (GET_INSTANCE(CoreRandomManager<size_t>))
#define CORE_RANDOM_MANAGER_FLOAT (GET_INSTANCE(CoreRandomManager<float>))

template<typename T>
class CoreRandomManager
{
	DECLARE_SINGLETON(CoreRandomManager<T>)

	using uniform_distribution = typename std::conditional<std::is_floating_point<T>::value, std::uniform_real_distribution<T>,
		                         typename std::conditional<std::is_integral<T>::value, std::uniform_int_distribution<T>, void>::type>::type;
public:
	bool GetRandomByPercent(const T percent, const T maxPercent);
	bool GetRandomByPercent(const double percent = 0.5);
	T GetRandom(const T from, const T to);
	size_t GetRandom(const CoreVector<T>& percentList);
};

IMPLEMENT_TEMPLATE_SINGLETON(CoreRandomManager)

template<typename T>
void CoreRandomManager<T>::Init(void)
{

}

template<typename T>
void CoreRandomManager<T>::Release(void)
{
	GetInstance().~CoreRandomManager();
}

template<typename T>
bool CoreRandomManager<T>::GetRandomByPercent(const T percent, const T maxPercent)
{
	return GetRandomByPercent(static_cast<double>(percent / maxPercent));
}

template<typename T>
bool CoreRandomManager<T>::GetRandomByPercent(const double percent)
{
	static thread_local std::mt19937 generator(std::random_device{}());
	std::bernoulli_distribution distribution(percent);
	return distribution(generator);
}

template <typename T>
T CoreRandomManager<T>::GetRandom(const T from, const T to)
{
	static thread_local std::mt19937 generator(std::random_device{}());
	uniform_distribution distribution(from, to);
	return distribution(generator);
}

template<typename T>
size_t CoreRandomManager<T>::GetRandom(const CoreVector<T>& percentList)
{
	static thread_local std::mt19937 generator(std::random_device{}());
	std::discrete_distribution<T> distribution{ percentList.cbegin(), percentList.cend() };
	return distribution(generator);
}