#include "CoreInclude.h"

IMPLEMENT_SINGLETON(CoreThreadManager)

void CoreThreadManager::Init(void)
{
	this->threads.push_back(std::thread([&]() { CORE_TIME_DELEGATE_MANAGER.Run(); }));
}

void CoreThreadManager::Release(void)
{
	GetInstance().~CoreThreadManager();
}


void CoreThreadManager::Push(std::function<void()> func)
{
	this->threads.push_back(std::thread(func));
}

void CoreThreadManager::Run(void)
{
	for (auto& d : this->threads)
		d.join();
}
