#include "../../Core/Core/CoreInclude.h"

#include <vector>
#include <array>
#include <algorithm>
#include <map>
#include <list>

class Object
{
public:
	Object()
	{
		CORE_LOG.Log("Default Construct Object");
	}
	Object(int v) : value(v)
	{
		CORE_LOG.Log("Construct Object");
	}
	virtual ~Object()
	{
		CORE_LOG.Log("Destruct Object");
	}

	Object(const Object& o) : value(o.GetValue())
	{
		CORE_LOG.Log("Copy Object");
	}
	Object(const Object&& o) : value(o.GetValue())
	{
		CORE_LOG.Log("Move Object");
	}

	int GetValue() const { return value; }
private:
	int value;
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

CoreList<int> data;
std::list<int> data2;

std::shared_mutex mutex1;
std::shared_mutex mutex2;

void DoTest(void)
{
	data.push_back(1);
}

void DoTest2(void)
{
	WRITE_LOCK(mutex2);
	data2.push_back(1);
}

int main(void)
{
#if _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif // _DEBUG
	//CoreVector<int> v;
	//v.push_back(1);
	//v.push_back(0);
	//v.push_back(5);
	//v.push_back(5);
	//v.push_back(5);
	//CoreBubbleSort(v.begin(), v.end());

	//for (int i = 0; i < v.size(); ++i)
	//	CORE_LOG.Log(TO_STR(v[i]));

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