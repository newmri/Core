#include "CoreInclude.h"

CoreGameObj::CoreGameObj()
{
	Init();
}

CoreGameObj::~CoreGameObj()
{

}

void CoreGameObj::Init(void)
{

}

CoreGameObj::CoreGameObj(const int64_t uid, const uint8_t ObjType) : CoreUIDObj(uid), objType(objType)
{
	Init();
}

uint8_t CoreGameObj::GetObjType(void)
{
	return this->objType;
}