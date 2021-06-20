#include "../../Core/Core/CoreInclude.h"

#include <vector>

class Test
{
	DECLARE_MEMORY_POOL(Test);

public:
	int i = 0;
	int k = 0;
};

IMPLEMENAT_MEMORY_POOL(Test);

class Test2
{

public:
	int i = 0;
	int k = 0;
};

void DoTest(size_t threadID)
{
	Test* test = new Test;
	delete test;
}

void DoTest2(size_t threadID)
{
	Test2* test2 = new Test2;
	delete test2;
}

int main(void)
{
#if _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif // _DEBUG

	Test::InitMemoryPool();

	CORE_TIME_MANAGER.Start();

	for (int i = 0; i < 100; ++i)
	{
		std::vector<std::thread> threads;

		for (size_t j = 0; j < 16; ++j)
			threads.emplace_back(std::thread([=]() { DoTest(j); }));

		for (auto& d : threads)
			d.join();
	}

	CORE_TIME_MANAGER.End();

	CORE_TIME_MANAGER.Start();

	for (int i = 0; i < 100; ++i)
	{
		std::vector<std::thread> threads;

		for (size_t j = 0; j < 16; ++j)
			threads.emplace_back(std::thread([=]() { DoTest(j); }));

		for (auto& d : threads)
			d.join();
	}

	CORE_TIME_MANAGER.End();
	return 0;
}