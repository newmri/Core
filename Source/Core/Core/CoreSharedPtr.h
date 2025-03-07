#pragma once

template<typename T>
class CoreSharedPtr
{
public:
	CoreSharedPtr();
	CoreSharedPtr(const CoreSharedPtr<T>& other);
	CoreSharedPtr(T* blockBody, const size_t needBlockNum = 1);
	~CoreSharedPtr();

public:
	T* operator->();
	T& operator*();
	T& operator[] (const size_t index);
	const T& operator[] (const size_t index) const;
	CoreSharedPtr<T>& operator=(const CoreSharedPtr<T>& other);
	operator bool() const;

public:
	T* get(void);
	bool IsValid(void);

private:
	size_t allocatedBlockNum;
	static std::atomic<size_t> refNum;
	T* blockBody;
};

#include "CoreSharedPtr.hpp"

template<typename T, typename... Types>
CoreSharedPtr<T> core_make_shared(const size_t needBlockNum = 1, Types... args)
{
	if(IS_SAME(1, needBlockNum))
		return CoreSharedPtr<T>(new T{ args... });

	return CoreSharedPtr<T>(new T[needBlockNum]{ args... });
}

class MultipleInheritableEnableSharedFromThis : public std::enable_shared_from_this<MultipleInheritableEnableSharedFromThis>
{
public:
	virtual ~MultipleInheritableEnableSharedFromThis()
	{
	}
};

template <class T>
class inheritable_enable_shared_from_this : virtual public MultipleInheritableEnableSharedFromThis
{
public:
	std::shared_ptr<T> shared_from_this()
	{
		return std::dynamic_pointer_cast<T>(MultipleInheritableEnableSharedFromThis::shared_from_this());
	}

	template <class Down>
	std::shared_ptr<Down> downcasted_shared_from_this()
	{
		return std::dynamic_pointer_cast<Down>(MultipleInheritableEnableSharedFromThis::shared_from_this());
	}
};