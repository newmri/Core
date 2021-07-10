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
	int i[100];
};


class Test2
{
public:
	int i[100];
};



void DoTest(const size_t threadID)
{
	Test* test = GET_INSTANCE(CoreMemoryPoolManager<Test>).Alloc(10000, CORE_RANDOM_MANAGER.GetRandom(9000, 10000));

	if (test)
		GET_INSTANCE(CoreMemoryPoolManager<Test>).DeAlloc(test);
}

void DoTest2(const size_t threadID)
{
	Test2* test2 = new Test2[CORE_RANDOM_MANAGER.GetRandom(100, 10000)];
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
		{
			std::thread th(DoTest, j);
			threads.push_back(std::move(th));
		}

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
		{
			std::thread th(DoTest, j);
			threads.push_back(std::move(th));
		}

		for (auto& d : threads)
			d.join();
	}

	CORE_TIME_MANAGER.End();
	return 0;
}