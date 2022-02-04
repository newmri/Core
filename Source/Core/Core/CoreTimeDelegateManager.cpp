#include "CoreInclude.h"

IMPLEMENT_SINGLETON(CoreTimeDelegateManager)

void CoreTimeDelegateManager::Init(void)
{

}

void CoreTimeDelegateManager::Release(void)
{
	GetInstance().~CoreTimeDelegateManager();
}

void CoreTimeDelegateManager::Push(CoreTimeDelegate<int, int> func)
{
	this->queueTwoIntArguments.push(func);
}

void CoreTimeDelegateManager::Run(void)
{
	while (true)
	{
		if (!this->queueTwoIntArguments.empty())
		{
			if (this->queueTwoIntArguments.top()->Run())
				this->queueTwoIntArguments.pop();
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}
