#pragma once

template<typename T>
struct CoreNode : public CoreMemoryPoolObj<CoreNode<T>, 1024>
{
	CoreNode() CORE_DEFAULT;

	CoreNode(const T& data) : data(data) {}

	CoreNode(const T& data, CoreNode<T>* next) : data(data), next(next) {}

	T data;
	CoreNode* next = nullptr;
};

template<typename T>
class CoreIterator
{
public:
	CoreIterator(CoreNode<T>* head = nullptr) : currNode(head) {}

	const CoreIterator& operator++() const
	{
		this->currNode = this->currNode->next;
		return *this;
	}

	CoreIterator& operator++()
	{
		return const_cast<CoreIterator&>(static_cast<const CoreIterator<T>&>(*this).operator++());
	}

	const T& operator*() const
	{
		return this->currNode->data;
	}

	T& operator*()
	{
		return const_cast<T&>(static_cast<const CoreIterator<T>&>(*this).operator*());
	}

	bool operator==(const CoreIterator& rhs) const
	{
		return currNode == rhs.currNode;
	}

	bool operator!=(const CoreIterator& rhs) const
	{
		return currNode != rhs.currNode;
	}

private:
	mutable CoreNode<T>* currNode = nullptr;
};
