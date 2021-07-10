#include "../../Core/Core/CoreInclude.h"

#include <vector>

class Test
{
public:
	//Test()
	//{
	//	//std::cout << "Ctor" << std::endl;
	//}

	//Test(int k)
	//{
	//	std::cout << "Ctor k" << std::endl;
	//}

	//~Test()
	//{
	//	//std::cout << "Detor" << std::endl;
	//}

public:
	int i[10000];
};


CoreMemoryPool<Test> p(1000);

class Test2
{
public:
	int i[10000];
};



void DoTest(const size_t threadID)
{
	Test* test = GET_INSTANCE(CoreMemoryPoolManager<Test>).Alloc(10, rand() % 10);
	
	//if (rand() % 2)
	//	GET_INSTANCE(CoreMemoryPoolManager<Test>).DeAlloc(test);
}

void DoTest2(const size_t threadID)
{
	Test2* test2 = new Test2[rand() % 10];
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

	CORE_LOG.Log(LogType::LOG_INFO, "page num: " + std::to_string(GET_INSTANCE(CoreMemoryPoolManager<Test>).GetPageNum()));

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