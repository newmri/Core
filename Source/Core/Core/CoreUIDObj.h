#pragma once

class CoreUIDObj : public CoreObj
{
	OVERRIDE_OBJ(CoreUIDObj)

public:
	CoreUIDObj(const int64_t uid);

public:
	int64_t GetUID(void);

protected:
	int64_t uid = 0;
};