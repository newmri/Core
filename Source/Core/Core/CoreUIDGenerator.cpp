#include "CoreInclude.h"

IMPLEMENT_SINGLETON(CoreUIDGenerator)

void CoreUIDGenerator::Init(void)
{

}

void CoreUIDGenerator::Release(void)
{
	GetInstance().~CoreUIDGenerator();
}

int64_t CoreUIDGenerator::GetUID(const DBInfo& dbInfo)
{
	TIME_VALUE now = 0;
	uint16_t sequence = 0;

	{
		std::lock_guard<std::mutex> lock(this->mutex);

		now = CORE_TIME_MANAGER.GetNowSeconds();

		if (now == this->lastTime)
		{
			if (this->sequence >= MAX_SEQUENCE_NUM)
			{
				while (now == this->lastTime)
					now = CORE_TIME_MANAGER.GetNowSeconds();

				this->sequence = 0;
			}
		}
		else
			this->sequence = 0;

		this->lastTime = now;
		sequence = this->sequence++;
	}


	uint64_t uid =
		(static_cast<uint64_t>(now) & MAX_TIMESTAMP) << (COUNTRY_BITS + WORLD_BITS + DB_BITS + SEQUENCE_BITS) |
		(static_cast<uint64_t>(dbInfo.countryCode) & MAX_COUNTRY_NUM) << (WORLD_BITS + DB_BITS + SEQUENCE_BITS) |
		(static_cast<uint64_t>(dbInfo.worldID) & MAX_WORLD_NUM) << (DB_BITS + SEQUENCE_BITS) |
		(static_cast<uint64_t>(dbInfo.dbID) & MAX_DB_NUM) << SEQUENCE_BITS |
		(static_cast<uint64_t>(sequence) & MAX_SEQUENCE_NUM);

	uid &= INT64_MAX; // 63비트 유지, 부호 비트 0으로 설정

	return static_cast<int64_t>(uid);
}

void CoreUIDGenerator::ParseUID(const int64_t uid) const
{
	uint64_t rawID = static_cast<uint64_t>(uid);

	uint16_t sequence = rawID & MAX_SEQUENCE_NUM;
	rawID >>= SEQUENCE_BITS;

	DBInfo dbInfo;

	dbInfo.dbID = rawID & MAX_DB_NUM;
	rawID >>= DB_BITS;

	dbInfo.worldID = rawID & MAX_WORLD_NUM;
	rawID >>= WORLD_BITS;

	dbInfo.countryCode = static_cast<CountryCode>(rawID & MAX_COUNTRY_NUM);
	rawID >>= COUNTRY_BITS;

	TIME_VALUE now;
	now = rawID & MAX_TIMESTAMP;
}
