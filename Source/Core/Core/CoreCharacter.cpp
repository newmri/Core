#include "CoreInclude.h"

CoreCharacter::CoreCharacter(const int64_t& accountUID, const int64_t& uid) : accountUID(accountUID), uid(uid)
{

}

int64_t CoreCharacter::GetAccountUID(void)
{
	return this->accountUID;
}

int64_t CoreCharacter::GetUID(void)
{
	return this->uid;
}