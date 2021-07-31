#pragma once

#include "CoreList.h"

template<typename T>
CoreList<T>::CoreList()
{
	
}

template<typename T>
CoreList<T>::~CoreList()
{

}

template<typename T>
void CoreList<T>::Init(void)
{
	CoreList<T>::Init();
}

template<typename T>
CoreList<T>::CoreList(const CoreList<T>& rhs)
{
	Copy(rhs);
}

template<typename T>
CoreList<T>& CoreList<T>::operator=(const CoreList<T>& rhs)
{
	Copy(rhs);
	return *this;
}

template<typename T>
void CoreList<T>::Copy(const CoreList<T>& rhs)
{

}

template<typename T>
void CoreList<T>::clear(void)
{
}

template<typename T>
void CoreList<T>::push_back(const T& data)
{
}

template<typename T>
void CoreList<T>::push_back(T&& data)
{
}
