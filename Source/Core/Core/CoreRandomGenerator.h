#pragma once

#include <random>

#define CORE_RANDOM_MANAGER_INT (GET_INSTANCE(CoreRandomGenerator<int32_t>))
#define CORE_RANDOM_MANAGER_SIZE_T (GET_INSTANCE(CoreRandomGenerator<size_t>))
#define CORE_RANDOM_MANAGER_TIME (GET_INSTANCE(CoreRandomGenerator<TIME_VALUE>))

#define CORE_RANDOM_MANAGER_FLOAT (GET_INSTANCE(CoreRandomGenerator<float_t>))

template<typename T>
class CoreRandomGenerator
{
	DECLARE_SINGLETON(CoreRandomGenerator<T>)

	using uniform_distribution = typename std::conditional<std::is_floating_point<T>::value, std::uniform_real_distribution<T>,
		                         typename std::conditional<std::is_integral<T>::value, std::uniform_int_distribution<T>, void>::type>::type;
public:
	bool GetRandomByPercent(const T percent, const T maxPercent);
	bool GetRandomByPercent(const double_t percent = 0.5);
	T GetRandom(const T from, const T to);
	size_t GetRandom(const CoreVector<T>& percentList);
};

IMPLEMENT_TEMPLATE_SINGLETON(CoreRandomGenerator)

template<typename T>
void CoreRandomGenerator<T>::Init(void)
{

}

template<typename T>
void CoreRandomGenerator<T>::Release(void)
{
	GetInstance().~CoreRandomGenerator();
}

template<typename T>
bool CoreRandomGenerator<T>::GetRandomByPercent(const T percent, const T maxPercent)
{
	return GetRandomByPercent(static_cast<double_t>(percent / maxPercent));
}

template<typename T>
bool CoreRandomGenerator<T>::GetRandomByPercent(const double_t percent)
{
	static thread_local std::mt19937 generator(std::random_device{}());
	std::bernoulli_distribution distribution(percent);
	return distribution(generator);
}

template <typename T>
T CoreRandomGenerator<T>::GetRandom(const T from, const T to)
{
	static thread_local std::mt19937 generator(std::random_device{}());
	uniform_distribution distribution(from, to);
	return distribution(generator);
}

template<typename T>
size_t CoreRandomGenerator<T>::GetRandom(const CoreVector<T>& percentList)
{
	static thread_local std::mt19937 generator(std::random_device{}());
	std::discrete_distribution<T> distribution{ percentList.cbegin(), percentList.cend() };
	return distribution(generator);
}