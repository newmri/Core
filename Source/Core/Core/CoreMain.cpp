#include "../../Core/Core/CoreInclude.h"

#include <vector>

class Test
{
public:
	int i = 0;
};

class Test2
{

public:
	int i = 0;
};

static const size_t max = 10;

CoreMemoryPool<Test> p(max);

void DoTest(size_t threadID)
{
	Test* test = p.Alloc(max);

	for(size_t i = 0; i < max; ++i)
		std::cout << test[i].i << std::endl;
	//if (test)
	//	p.DeAlloc(test);
}

void DoTest2(size_t threadID)
{
	Test2* test2 = new Test2;
	if(test2)
		delete test2;
}

int main(void)
{
#if _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif // _DEBUG

	DoTest(1);

	//CORE_TIME_MANAGER.Start();

	//for (int i = 0; i < 10; ++i)
	//{
	//	std::vector<std::thread> threads;

	//	for (size_t j = 0; j < 16; ++j)
	//		threads.emplace_back(std::thread([=]() { DoTest(j); }));

	//	for (auto& d : threads)
	//		d.join();
	//}

	//CORE_TIME_MANAGER.End();

	//CORE_TIME_MANAGER.Start();

	//for (int i = 0; i < 10; ++i)
	//{
	//	std::vector<std::thread> threads;

	//	for (size_t j = 0; j < 16; ++j)
	//		threads.emplace_back(std::thread([=]() { DoTest2(j); }));

	//	for (auto& d : threads)
	//		d.join();
	//}

	//CORE_TIME_MANAGER.End();
	return 0;
}