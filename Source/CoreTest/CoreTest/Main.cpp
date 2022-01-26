#include "../../Core/Core/CoreInclude.h"

#include <vector>
#include <array>
#include <algorithm>
#include <map>
#include <list>
#include <queue>

class Object : public CoreMemoryPoolManager<Object>
{
public:

	Object()
	{
		value = 0;
		std::cout << "aa" << std::endl; 
	}

	~Object()
	{
		std::cout << "bb" << std::endl;
	}

	int GetValue() const { return value; }
public:
	int value;
};

template<typename T>
class CoreSharedPtr
{
public:
	CoreSharedPtr(T* target) : target(target)
	{

	}

	~CoreSharedPtr()
	{
		delete target;
		target = nullptr;
	}

	bool IsValid(void)
	{
		if (IS_SAME(nullptr, target))
			return false;

		return target->IsValid(target);
	}

	T* operator->()
	{
		return target;
	}

private:
	T* target;
};

class Test
{
	int i[100];
};

class Test2
{
public:
	int i[100];
};

class Test3
{
public:
	int i[100];
};

//CoreList<int> data;
//std::list<int> data2;
//
//std::shared_mutex mutex1;
//std::shared_mutex mutex2;
//
//void DoTest(void)
//{
//	data.push_back(1);
//}
//
//void DoTest2(void)
//{
//	WRITE_LOCK(mutex2);
//	data2.push_back(1);
//}

void Attack(int attacker, int victim)
{
	CORE_LOG.Log(TO_STR(attacker) + " is attacking " + TO_STR(victim));
}

int main(void)
{
#if _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif // _DEBUG

	CoreSharedPtr<Object> t(new Object);

	if (t.IsValid())
		std::cout << t->value << std::endl;

	//delete p;
	//p = nullptr;

	//p2->value = 3;

	//CorePriorityQueue<CoreTimeDelegate<int, int>> q;
	//q.push(CoreTimeDelegate<int, int>(Attack, 0, 1));
	//q.push(CoreTimeDelegate<int, int>(Attack, 2, 3, 5000));
	//q.push(CoreTimeDelegate<int, int>(Attack, 1, 2, 3000));
	//q.push(CoreTimeDelegate<int, int>(Attack, 10, 11, 7000, 1000, 10));

	//while (true)
	//{
	//	if (!q.empty())
	//	{
	//		if (q.top()->Run())
	//			q.pop();
	//	}
	//}

	//CORE_LOG.Log("----------- mine -----------------");
	//CORE_TEST_MANAGER.Work(DoTest);
	//CORE_LOG.Log("size: " + TO_STR(data.size()));

	//CORE_LOG.Log("Memory Page: " + TO_STR(GET_INSTANCE(CoreMemoryPoolManager<CoreNode<int>>).GetPageNum()));
	//CORE_LOG.Log("");

	//CORE_LOG.Log("----------- stl -----------------");
	//CORE_TEST_MANAGER.Work(DoTest2);
	//CORE_LOG.Log("size: " + TO_STR(data2.size()));

	return 0;
}