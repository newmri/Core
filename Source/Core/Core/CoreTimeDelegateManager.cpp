#include "CoreInclude.h"

IMPLEMENT_SINGLETON(CoreTimeDelegateManager)

void CoreTimeDelegateManager::Init(void)
{

}

void CoreTimeDelegateManager::Release(void)
{
	GetInstance().~CoreTimeDelegateManager();
}

void CoreTimeDelegateManager::Push(CoreTimeDelegate<> func)
{
	this->queueNoneArguments.push(func);
}

void CoreTimeDelegateManager::Push(CoreTimeDelegate<void*> func)
{
	this->queueOnePointerArguments.push(func);
}

void CoreTimeDelegateManager::Push(CoreTimeDelegate<int32_t> func)
{
	this->queueOneIntArguments.push(func);
}

void CoreTimeDelegateManager::Push(CoreTimeDelegate<int32_t, int32_t> func)
{
	this->queueTwoIntArguments.push(func);
}

void CoreTimeDelegateManager::Run(void)
{
	while (IsRunning)
	{
		if (!this->queueNoneArguments.empty())
		{
			if (this->queueNoneArguments.top()->Run())
				this->queueNoneArguments.pop();
		}

		if (!this->queueOnePointerArguments.empty())
		{
			if (this->queueOnePointerArguments.top()->Run())
				this->queueOnePointerArguments.pop();
		}

		if (!this->queueOneIntArguments.empty())
		{
			if (this->queueOneIntArguments.top()->Run())
				this->queueOneIntArguments.pop();
		}

		if (!this->queueTwoIntArguments.empty())
		{
			if (this->queueTwoIntArguments.top()->Run())
				this->queueTwoIntArguments.pop();
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}

void CoreTimeDelegateManager::Stop(void)
{
	IsRunning = false;
}
