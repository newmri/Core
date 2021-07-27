#include "CoreInclude.h"

IMPLEMENT_SINGLETON(CoreDummyManager)

void CoreDummyManager::Init(void)
{
	dummyString = std::string(this->defaultLogSize, '\0');
}

void CoreDummyManager::Release(void)
{
	GetInstance().~CoreDummyManager();
}

std::string_view CoreDummyManager::GetDummyString(void) const
{
	return dummyString;
}