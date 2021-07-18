#pragma once

template<typename T>
class CoreContainer : public CoreObj
{
	OVERRIDE_OBJ(CoreContainer<T>)

public:
	template<typename... Types>
	CoreContainer(const size_t maxBlockNum, const size_t needBlockNum, const bool needCallCtor = true, Types... args);

protected:
	template<typename... Types>
	T* Alloc(const size_t maxBlockNum, const size_t needBlockNum, const bool needCallCtor = true, Types... args);
	void DeAlloc(void);

public:
	virtual void clear(void) CORE_PURE;

public:
	size_t size(void);

public:
	bool empty(void);

protected:
	void SetSize(const size_t newSize);

protected:
	size_t dataSize = 0;
	T* data = nullptr;

protected:
	std::shared_mutex mutex;
};

template<typename T>
CoreContainer<T>::CoreContainer()
{
}

template<typename T>
CoreContainer<T>::~CoreContainer()
{
	WRITE_LOCK(this->mutex);
	DeAlloc();
}

template<typename T>
template<typename... Types>
CoreContainer<T>::CoreContainer(const size_t maxBlockNum, const size_t needBlockNum, const bool needCallCtor, Types... args)
{
	this->data = Alloc(maxBlockNum, needBlockNum, needCallCtor, args...);
}

template<typename T>
void CoreContainer<T>::Init(void)
{
}

template<typename T>
template<typename... Types>
T* CoreContainer<T>::Alloc(const size_t maxBlockNum, const size_t needBlockNum, const bool needCallCtor, Types... args)
{
	T* body = GET_INSTANCE(CoreMemoryPoolManager<T>).Alloc(maxBlockNum, needBlockNum, needCallCtor, args...);

	if (body)
		SetSize(this->dataSize + needBlockNum);

	return body;
}

template<typename T>
void CoreContainer<T>::DeAlloc(void)
{
	if (this->data)
	{
		GET_INSTANCE(CoreMemoryPoolManager<T>).DeAlloc(this->data, false);
		SetSize(0);
	}
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