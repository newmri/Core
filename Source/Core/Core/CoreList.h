#pragma once

template<typename T>
class CoreList : public CoreQueue<T>
{
public:
	CoreList() CORE_DEFAULT;
	CoreList(CoreList<T>& rhs);
	virtual ~CoreList() override;

public:
	CoreList<T>& operator=(CoreList<T>& rhs);

private:
	void Copy(const CoreList<T>& rhs);

public:
	void push_front(const T& data);
	void push_front(T&& data);
	void push_back(const T& data);
	void push_back(T&& data);
	void remove(const T& data);
	template<typename FUNC = bool(*)(const T&, const T&)>
	void remove_if(FUNC Compare);

private:
	void push_front(CoreNode<T>* newNode);

public:
	typedef CoreIterator<T> iterator;
	typedef const CoreIterator<T> const_iterator;
};

#include "CoreList.hpp"