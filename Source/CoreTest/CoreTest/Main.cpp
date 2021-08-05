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

void DoTest(const size_t threadID)
{
	for (int i = 0; i < 100; ++i)
	{
		if(CORE_RANDOM_MANAGER_INT.GetRandomByPercent(0.5))
			data.push_back(i);
	}
}

void DoTest2(const size_t threadID)
{
	for (int i = 0; i < 100; ++i)
	{
		WRITE_LOCK(mutex2);

		if (CORE_RANDOM_MANAGER_INT.GetRandomByPercent(0.5))
			data2.push_back(i);
	}
}

void Test(void (*fp)(const size_t))
{
	CORE_TIME_MANAGER.Start();

	for (size_t i = 0; i < 100; ++i)
	{
		std::vector<std::thread> threads;

		for (size_t j = 0; j < 10; ++j)
		{
			std::thread th(fp, j);
			threads.push_back(std::move(th));
		}

		for (auto& d : threads)
			d.join();
	}

	CORE_TIME_MANAGER.End();
}

int main(void)
{
#if _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif // _DEBUG

	//CORE_LOG.Log("----------- Test -----------------");
	//for (size_t i = 0; i < 3; ++i)
	//	Test(DoTest2);

	//CoreRandomManager<int>::GetInstance().GetRandom(v);

	CORE_LOG.Log("----------- stl -----------------");

	for (size_t i = 0; i < 2; ++i)
		Test(DoTest2);

	CORE_LOG.Log(TO_STR(data2.size()));

	CORE_LOG.Log("----------- mine -----------------");

	for (size_t i = 0; i < 2; ++i)
		Test(DoTest);

	CORE_LOG.Log(TO_STR(data.size()));
	CORE_LOG.Log("Memory Page: " + TO_STR(GET_INSTANCE(CoreMemoryPoolManager<CoreNode<int>>).GetPageNum()));


	return 0;
}