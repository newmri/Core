#pragma once

class CoreCharacter : public CoreMemoryPoolObj<CoreCharacter, CORE_BIG_SIZE>, public std::enable_shared_from_this<CoreCharacter>
{
public:
	CoreCharacter(const int64_t& accountUID, const int64_t& uid);
	virtual ~CoreCharacter();

public:
	int64_t GetAccountUID(void);
	int64_t GetUID(void);

protected:
	int64_t accountUID;
	int64_t uid;
};