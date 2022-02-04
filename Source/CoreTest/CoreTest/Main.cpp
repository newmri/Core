#include "../../Core/Core/CoreInclude.h"

#include <vector>
#include <array>
#include <algorithm>
#include <map>
#include <list>
#include <queue>

class Object : public CoreMemoryPoolObj<Object, 1024>
{
public:

	Object()
	{
		CORE_LOG.Log("ctor");
		value = 0;
	}

	Object(int value) : value(value)
	{
		CORE_LOG.Log("ctor2");

	}

	virtual ~Object()
	{
		CORE_LOG.Log("detor");

	}

	int GetValue() const { return value; }
public:
	int value;
};

class Object2
{
public:

	Object2()
	{
		value = 0;
	}

	Object2(int value) : value(value)
	{
	}

	virtual ~Object2()
	{
	}

	int GetValue() const { return value; }
public:
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


CoreVector<int> data;
std::vector<int> data2;

std::shared_mutex mutex1;
std::shared_mutex mutex2;

void DoTest(void)
{
	data.push_back(1);
	//data.
}

void DoTest2(void)
{
	WRITE_LOCK(mutex2);
	data2.push_back(1);
	//data2.pop();
}

void Attack(int attacker, int victim)
{
	CORE_LOG.Log(TO_STR(attacker) + " is attacking " + TO_STR(victim));
}

void Run()
{
	CORE_LOG.Log("----------- mine -----------------");
	CORE_TEST_MANAGER.Work(DoTest);
	CORE_LOG.Log("size: " + TO_STR(data.size()));

	CORE_LOG.Log("----------- stl -----------------");
	CORE_TEST_MANAGER.Work(DoTest2);
	CORE_LOG.Log("size: " + TO_STR(data2.size()));
}

int main(void)
{
#if _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif // _DEBUG

	CORE_THREAD_MANAGER.Push(Run);
	CORE_THREAD_MANAGER.Run();

	return 0;
}