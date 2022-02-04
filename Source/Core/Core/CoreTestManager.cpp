#include "CoreInclude.h"

IMPLEMENT_SINGLETON(CoreTestManager)

void CoreTestManager::Init(void)
{
	
}

void CoreTestManager::Release(void)
{
	GetInstance().~CoreTestManager();
}

void CoreTestManager::Work(void (*func)(void), TestInfo testInfo)
{
	size_t testNumber = testInfo.testNumber;
	size_t size = 0;

	size_t threadNumber = testInfo.startThreadNumber;

	for (; threadNumber <= testInfo.endThreadNumber; threadNumber *= testInfo.increaseThreadNumberTimes)
	{
		testInfo.testNumber = testNumber / threadNumber;

		std::vector<std::thread> threads;

		for (size_t i = 0; i < threadNumber; ++i)
			threads.emplace_back(std::thread([&]()
				{
					for (size_t j = 0; j < testInfo.testNumber; ++j)
						func();
				}));

		CORE_ALL_LOG(LogType::LOG_INFO, "[threadNumber]: " + TO_STR(threadNumber));

		CORE_TIME_MANAGER.Start();

		for (auto& d : threads)
			d.join();

		CORE_TIME_MANAGER.End();
	}
}