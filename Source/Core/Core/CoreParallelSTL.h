#pragma once

template<typename T>
class CoreParallelSTL
{
public:
	CoreParallelSTL() CORE_DEFAULT;
	CoreParallelSTL(const size_t size);
	virtual ~CoreParallelSTL() CORE_DEFAULT;

public:
	virtual void Clear(void);

public:
	size_t GetSize(void);

protected:
	void SetSize(const size_t newSize);

protected:
	size_t size = 0;

protected:
	std::shared_mutex mutex;
};

template<typename T>
CoreParallelSTL<T>::CoreParallelSTL(const size_t size)
{
}


template<typename T>
void CoreParallelSTL<T>::Clear(void)
{
	SetSize(0);
};

template<typename T>
size_t CoreParallelSTL<T>::GetSize(void)
{
	READ_LOCK(this->mutex);
	return this->size;
}

template<typename T>
void CoreParallelSTL<T>::SetSize(const size_t newSize)
{
	this->size = newSize;
}