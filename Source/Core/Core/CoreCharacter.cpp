#include "CoreInclude.h"

CoreCharacter::~CoreCharacter()
{

}

void CoreCharacter::Init(void)
{

}

CoreCharacter::CoreCharacter(const int64_t& accountUID, const int64_t& uid) : accountUID(accountUID), CoreUIDObject(uid)
{
	Init();
}

int64_t CoreCharacter::GetAccountUID(void)
{
	return this->accountUID;
}