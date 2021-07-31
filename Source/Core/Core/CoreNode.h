#pragma once

template<typename T>
struct CoreNode
{
	CoreNode() CORE_DEFAULT;
	CoreNode(T data) : data(data) {}

private:
	T data;
	CoreNode* next = nullptr;
};