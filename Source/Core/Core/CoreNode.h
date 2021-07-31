#pragma once

template<typename T>
struct CoreNode
{
	CoreNode() CORE_DEFAULT;

	CoreNode(const T& data, CoreNode<T>* next) : data(data), next(next) {}

	T data;
	CoreNode* next = nullptr;
};

template<typename T>
class CoreIterator
{
public:
	CoreIterator(CoreNode<T>* head = nullptr) : currNode(head) {}

	CoreIterator& operator++()
	{
		this->currNode = this->currNode->next;
		return *this;
	}

	T& operator*()
	{
		return this->currNode->data;
	}

	bool operator==(CORE_REF(CoreIterator) rhs) const
	{
		return currNode == rhs.currNode;
	}

	bool operator!=(CORE_REF(CoreIterator) rhs) const
	{
		return currNode != rhs.currNode;
	}

private:
	CoreNode<T>* currNode = nullptr;
};