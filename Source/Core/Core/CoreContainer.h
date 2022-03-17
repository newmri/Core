#pragma once

template<typename T>
class CoreContainer : public CoreObject
{
	OVERRIDE_OBJECT(CoreContainer<T>)

public:
	virtual void clear(void) CORE_PURE;

public:
	size_t size(void);

public:
	bool empty(void);

protected:
	void SetSize(const size_t newSize);

protected:
	CACHE_ALIGN size_t dataSize = 0;

protected:
	CACHE_ALIGN std::shared_mutex mutex;
};

template<typename T>
CoreContainer<T>::CoreContainer()
{
}

template<typename T>
CoreContainer<T>::~CoreContainer()
{
	WRITE_LOCK(this->mutex);
}

template<typename T>
void CoreContainer<T>::Init(void)
{
}

template<typename T>
size_t CoreContainer<T>::size(void)
{
	READ_LOCK(this->mutex);
	return this->dataSize;
}

template<typename T>
bool CoreContainer<T>::empty(void)
{
	return (0 == size());
}

template<typename T>
void CoreContainer<T>::SetSize(const size_t newSize)
{
	this->dataSize = newSize;
}