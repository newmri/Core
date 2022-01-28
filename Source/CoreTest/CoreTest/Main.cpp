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
		value = 0;
	}

	Object(int value) : value(value)
	{
	}

	virtual ~Object()
	{
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


CoreQueue<int> data;
std::queue<int> data2;

std::shared_mutex mutex1;
std::shared_mutex mutex2;

void DoTest(void)
{
	data.push(1);
	data.pop();
}

void DoTest2(void)
{
	WRITE_LOCK(mutex2);
	data2.push(1);
	data2.pop();
}

void Attack(int attacker, int victim)
{
	CORE_LOG.Log(TO_STR(attacker) + " is attacking " + TO_STR(victim));
}

int main(void)
{
#if _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif // _DEBUG

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

	//{
	//	CoreQueue<int> t;
	//	t.push(1);
	//}


	CORE_LOG.Log("----------- mine -----------------");
	CORE_TEST_MANAGER.Work(DoTest);
	CORE_LOG.Log("size: " + TO_STR(data.size()));
	
	CORE_LOG.Log("----------- stl -----------------");
	CORE_TEST_MANAGER.Work(DoTest2);
	CORE_LOG.Log("size: " + TO_STR(data2.size()));

	return 0;
}