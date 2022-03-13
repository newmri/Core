#pragma once

class CoreCharacter : public CoreUIDObj, public CoreMemoryPoolObj<CoreCharacter, CORE_BIG_SIZE>, public std::enable_shared_from_this<CoreCharacter>
{
	OVERRIDE_OBJ(CoreCharacter)

public:
	CoreCharacter(const int64_t& accountUID, const int64_t& uid);

public:
	int64_t GetAccountUID(void);

protected:
	int64_t accountUID;
};