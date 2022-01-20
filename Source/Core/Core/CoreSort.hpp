#pragma once

#include "CoreSort.h"

template<typename T>
void CoreSwap(T& left, T& right)
{
	T leftBackup = left;
	left = right;
	right = leftBackup;
}

template<typename T>
bool CoreAscendingOrder(const T& left, const T& right)
{
	return (left > right);
}

template<typename T>
bool CoreDescendingOrder(const T& left, const T& right)
{
	return (left < right);
}

template<typename T>
size_t CoreGetLen(T* start, T* end)
{
	if (start >= end)
	{
		std::string errorMessage = "start " + TO_STR(*start) + " is Greater or Same than end " + TO_STR(*end);
		CORE_LOG.Log(LogType::LOG_ERROR, errorMessage);
		return false;
	}

	return (end - start);
}

template<typename T, typename FUNC>
void CoreBubbleSort(T* start, T* end, FUNC Compare)
{
	size_t len = CoreGetLen(start, end);

	if (false == len) return;

	for (size_t i = 0; i < len; ++i)
	{
		for (size_t j = 0; j < len - i - 1; ++j)
		{
			if (Compare(start[j], start[j + 1]))
				CoreSwap(start[j], start[j + 1]);
		}
	}
}

template<typename T>
void CoreBubbleSort(T* start, T* end)
{
	CoreBubbleSort(start, end, CoreAscendingOrder);
}

template<typename T, typename FUNC>
void CoreSelectionSort(T* start, T* end, FUNC Compare)
{
	size_t len = CoreGetLen(start, end);

	if (false == len) return;

	size_t targetIndex = 0;

	for (size_t i = 0; i < len - 1; ++i)
	{
		targetIndex = i;

		for (size_t j = i + 1; j < len; ++j)
		{
			if (Compare(start[targetIndex], start[j]))
				targetIndex = j;
		}

		if (i != targetIndex)
		{
			CoreSwap(start[i], start[targetIndex]);
		}
	}
}

template<typename T>
void CoreSelectionSort(T* start, T* end)
{
	CoreSelectionSort(start, end, CoreAscendingOrder);
}

template<typename T, typename FUNC>
void CoreInsertionSort(T* start, T* end, FUNC Compare)
{
	size_t len = CoreGetLen(start, end);

	if (false == len) return;

	size_t targetIndex;

	T target;

	for (size_t i = 1; i < len; ++i)
	{
		target = start[i];

		for (targetIndex = i; targetIndex > 0; --targetIndex)
		{
			if (Compare(start[targetIndex - 1], target))
				start[targetIndex] = start[targetIndex - 1];

			else break;
		}

		start[targetIndex] = target;
	}
}

template<typename T>
void CoreInsertionSort(T* start, T* end)
{
	InsertionSort(start, end, CoreAscendingOrder);
}