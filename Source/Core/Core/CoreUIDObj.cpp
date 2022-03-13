#include "CoreInclude.h"

CoreUIDObj::~CoreUIDObj()
{

}

void CoreUIDObj::Init(void)
{

}

CoreUIDObj::CoreUIDObj(const int64_t uid) : uid(uid)
{
	Init();
}

int64_t CoreUIDObj::GetUID(void)
{
	return this->uid;
}