#include "../../Core/Core/CoreInclude.h"

#include <vector>
#include <array>
#include <algorithm>

class Test
{
	int i[100];
};


class Test2
{
public:
	int i[100];
};

CoreArray<size_t, 1000> arr;
std::array<size_t, 1000> arr2;

std::shared_mutex mutex1;
std::shared_mutex mutex2;

void DoTest(const size_t threadID)
{
	CoreArray<size_t, 1000> arrTemp;


	for (size_t i = 0; i < arrTemp.size(); ++i)
	{
		arrTemp[i] = i;
	}

	arr = arrTemp;
}

void DoTest2(const size_t threadID)
{
	std::array<size_t, 1000> arrTemp;

	for (size_t i = 0; i < arr2.size(); ++i)
	{
		arrTemp[i] = i;
	}

	arr2 = arrTemp;
}

void Test(void (*fp)(const size_t))
{
	CORE_TIME_MANAGER.Start();

	for (size_t i = 0; i < 100; ++i)
	{
		std::vector<std::thread> threads;

		for (size_t j = 0; j < 16; ++j)
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
	
	CoreArray<int, 10> ar{ 1 };

	for(auto d : ar)
		std::cout<<d<<std::endl;

	//CORE_LOG.Log("----------- Test -----------------");
	//for (size_t i = 0; i < 3; ++i)
	//	Test(DoTest2);

	//CORE_LOG.Log("----------- stl -----------------");

	//for (size_t i = 0; i < 10; ++i)
	//	Test(DoTest2);

	//CORE_LOG.Log("----------- mine -----------------");

	//for (size_t i = 0; i < 10; ++i)
	//	Test(DoTest);

	return 0;
}