#include "CoreInclude.h"

IMPLEMENT_SINGLETON(CoreUIDGenerator)

void CoreUIDGenerator::Init(void)
{

}

void CoreUIDGenerator::Release(void)
{
	GetInstance().~CoreUIDGenerator();
}

uint64_t CoreUIDGenerator::GetUID(const CountryCode countryID, const uint8_t worldID, const uint8_t dbID)
{
	TIME_VALUE now = 0;
	uint16_t sequence = 0;

	{
		std::lock_guard<std::mutex> lock(this->mutex);

		now = CORE_TIME_MANAGER.GetNowMilliSeconds();

		if (now == this->lastTime)
		{
			if (this->sequence >= MAX_SEQUENCE_NUM)
			{
				while (now == this->lastTime)
					now = CORE_TIME_MANAGER.GetNowMilliSeconds();

				this->sequence = 0;
			}
		}
		else
			this->sequence = 0;

		this->lastTime = now;
		sequence = this->sequence++;
	}


	return (now << (COUNTRY_BITS + WORLD_BITS + DB_BITS + SEQUENCE_BITS)) |
		(countryID << (WORLD_BITS + DB_BITS + SEQUENCE_BITS)) |
		(worldID << (DB_BITS + SEQUENCE_BITS)) |
		(dbID << SEQUENCE_BITS) |
		(sequence);
}