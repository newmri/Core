#include "CoreInclude.h"

CoreUIDObject::~CoreUIDObject()
{

}

void CoreUIDObject::Init(void)
{

}

CoreUIDObject::CoreUIDObject(const int64_t uid) : uid(uid)
{
	Init();
}

int64_t CoreUIDObject::GetUID(void) const
{
	return this->uid;
}