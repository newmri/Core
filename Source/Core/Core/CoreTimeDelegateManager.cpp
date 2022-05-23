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

void CoreTimeDelegateManager::Run(void)
{
	while (IsRunning)
	{
		if (!this->queueNoneArguments.empty())
		{
			if (this->queueNoneArguments.top()->Run())
				this->queueNoneArguments.pop();
		}
	}
}

void CoreTimeDelegateManager::Stop(void)
{
	IsRunning = false;
}
