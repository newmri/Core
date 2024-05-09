#include "CoreInclude.h"

CoreCharacter::~CoreCharacter()
{

}

void CoreCharacter::Init(void)
{
	CoreUIDObject::Init();
}

CoreCharacter::CoreCharacter(const int64_t& accountUID, const int64_t& uid) : accountUID(accountUID), CoreUIDObject(uid)
{

}

int64_t CoreCharacter::GetAccountUID(void)
{
	return this->accountUID;
}