#pragma once

#define CORE_UID_GENERATOR (GET_INSTANCE(CoreUIDGenerator))

static constexpr uint8_t TIME_STAMP_BITS = 34;
static constexpr uint8_t COUNTRY_BITS = 7;
static constexpr uint8_t WORLD_BITS = 5;
static constexpr uint8_t DB_BITS = 3;
static constexpr uint8_t SEQUENCE_BITS = 15;

static constexpr uint64_t MAX_TIMESTAMP = (1ULL << TIME_STAMP_BITS) - 1;
static constexpr uint64_t MAX_COUNTRY_NUM = (1ULL << COUNTRY_BITS) - 1;
static constexpr uint64_t MAX_WORLD_NUM = (1ULL << WORLD_BITS) - 1;
static constexpr uint64_t MAX_DB_NUM = (1ULL << DB_BITS) - 1;
static constexpr uint64_t MAX_SEQUENCE_NUM = (1ULL << SEQUENCE_BITS) - 1;

class CoreUIDGenerator
{
	DECLARE_SINGLETON(CoreUIDGenerator)

public:
	uint64_t GetUID(const CountryCode countryID, const uint8_t worldID, const uint8_t dbID);

private:
	std::mutex mutex;
	int64_t lastTime = 0;
	uint16_t sequence = 0;
};