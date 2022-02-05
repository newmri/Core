#include "../../Core/Core/CoreInclude.h"

#include "person_generated.h"

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
		CORE_LOG.Log("ctor");
		value = 0;
	}

	Object2(int value) : value(value)
	{
	}

	virtual ~Object2()
	{
		CORE_LOG.Log("detor");
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
	std::string str("", CORE_BIG_SIZE);
	str = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
	//data.push_back(1);
}

void DoTest2(void)
{
	std::string str;
	str.reserve(CORE_BIG_SIZE);
	str = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";

	//WRITE_LOCK(mutex2);
	//data2.push_back(1);
	//data2.pop();
}

void Attack(int attacker, int victim)
{
	CORE_LOG.Log(TO_STR(attacker) + " is attacking " + TO_STR(victim));
}

void Run()
{
	CORE_ALL_LOG(LogType::LOG_INFO, "----------- mine -----------------");
	CORE_TEST_MANAGER.Work(DoTest);
	CORE_ALL_LOG(LogType::LOG_INFO, "size: " + TO_STR(data.size()));
	data.clear();

	CORE_ALL_LOG(LogType::LOG_INFO, "----------- stl -----------------");
	CORE_TEST_MANAGER.Work(DoTest2);
	CORE_ALL_LOG(LogType::LOG_INFO, "size: " + TO_STR(data2.size()));
	data2.clear();
}

int main(void)
{
#if _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif // _DEBUG


	//CORE_TIME_DELEGATE_MANAGER.Push(CoreTimeDelegate<int, int>(Attack, 0, 1));
	//CORE_THREAD_MANAGER.Push(Run);

	//CORE_TIME_DELEGATE_MANAGER.Push(CoreTimeDelegate<>(Run));
	//CORE_THREAD_MANAGER.Run();

	CoreServer server(7777);
	server.Start();
	server.Stop();

	return 0;
}