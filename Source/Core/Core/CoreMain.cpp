#include "../../Core/Core/CoreInclude.h"

#include <vector>

class Test
{
public:
	int i[10000];
};

class Test2
{
public:
	int i[10000];
};

static const size_t max = 10000;

CoreMemoryPool<Test> p(max);

void DoTest(const size_t threadID)
{
	Test* test = p.Alloc(rand() % max);

	if (test)
		p.DeAlloc(test);

	else
		std::cout << "nullptr" << std::endl;
}

void DoTest2(const size_t threadID)
{
	Test2* test2 = new Test2[rand() % max];
	if(test2)
		delete[] test2;
}

int main(void)
{
#if _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif // _DEBUG

	CORE_TIME_MANAGER.Start();

	for (int i = 0; i < 100; ++i)
	{
		std::vector<std::thread> threads;

		for (size_t j = 0; j < 32; ++j)
			threads.emplace_back(std::thread([=]() { DoTest(j); }));

		for (auto& d : threads)
			d.join();
	}

	CORE_TIME_MANAGER.End();

	CORE_TIME_MANAGER.Start();

	for (int i = 0; i < 100; ++i)
	{
		std::vector<std::thread> threads;

		for (size_t j = 0; j < 32; ++j)
			threads.emplace_back(std::thread([=]() { DoTest2(j); }));

		for (auto& d : threads)
			d.join();
	}

	CORE_TIME_MANAGER.End();
	return 0;
}