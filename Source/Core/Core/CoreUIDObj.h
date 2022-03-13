#pragma once

class CoreUIDObj : public CoreObj
{
	OVERRIDE_OBJ_DELETE_DEFAULT_CONSTRUCTOR(CoreUIDObj)

public:
	CoreUIDObj(const int64_t uid);

public:
	int64_t GetUID(void) const;

protected:
	int64_t uid = 0;
};