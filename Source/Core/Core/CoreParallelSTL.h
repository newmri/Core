#pragma once

template<typename T>
class CoreParallelSTL
{
public:
	CoreParallelSTL() CORE_DEFAULT;
	CoreParallelSTL(const size_t newCapacity) : capacity(newCapacity) {}
	virtual ~CoreParallelSTL() CORE_DEFAULT;

public:
	virtual void Push(const T newData) CORE_PURE;
protected:
	virtual void Push(void) { SetSize(this->size + 1); }

public:
	virtual void Pop(void) { SetSize(this->size - 1); };
	virtual void Clear(void) { SetSize(0); };

public:
	size_t GetCapacity(void)
	{
		READ_LOCK(this->mutex);
		return this->capacity; 
	}

	size_t GetSize(void)
	{
		READ_LOCK(this->mutex);
		return this->size; 
	}

private:
	void SetSize(const size_t newSize)
	{
		WRITE_LOCK(mutex);
		this->size = newSize;
	}

	void SetCapacity(const size_t newCapacity)
	{
		WRITE_LOCK(mutex);
		this->capacity = newCapacity;
	}

protected:
	size_t capacity = 0;
	size_t size = 0;

protected:
	std::shared_mutex mutex;
};