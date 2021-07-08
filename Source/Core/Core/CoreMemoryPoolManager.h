#pragma once

template<typename T>
class CoreMemoryPoolManager
{
	DECLARE_SINGLETON(CoreMemoryPoolManager<T>)

public:
	template<typename... Types>
	T* Alloc(const size_t needBlockNum, Types... args);
};

IMPLEMENT_TEMPLATE_SINGLETON(CoreMemoryPoolManager)

template<typename T>
void CoreMemoryPoolManager<T>::Init(void)
{
	
}

template<typename T>
template<typename... Types>
T* CoreMemoryPoolManager<T>::Alloc(const size_t needBlockNum, Types... args)
{
	return nullptr;
}