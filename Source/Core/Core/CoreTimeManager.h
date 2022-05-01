#pragma once

#define CORE_TIME_MANAGER GET_INSTANCE(CoreTimeManager)

using TIME_POINT = std::chrono::system_clock::time_point;
using TIME_VALUE = int64_t;

static constexpr TIME_VALUE SEC = 1000;
static constexpr float_t SEC_FLOAT = 1000.0f;

static constexpr TIME_VALUE HALF_MIN = SEC * 30;
static constexpr TIME_VALUE MIN = HALF_MIN * 2;

class CoreTimeManager
{
	DECLARE_SINGLETON(CoreTimeManager)

private:
	TIME_POINT GetNow(void) const;

public:
	TIME_VALUE GetNowSeconds(void) const;
	TIME_VALUE GetNowMilliSeconds(void) const;
	TIME_VALUE GetEndSeconds(const TIME_VALUE limitTime) const;
	bool IsExpired(const TIME_VALUE time) const;

public:
	void Start(void);
	void End(void);

private:
	void GetTime(std::string_view format, std::string& time);

public:
	std::string GetDate(void);
	std::string GetTime(void);
	std::string GetDateTime(void);

private:
	TIME_POINT start, end;
	std::chrono::duration<double> sec;
	std::string timeEndMessage = "Time has been passed (sec) ";
	static const int32_t timeBufferSize = 128;
	char timeBuffer[timeBufferSize];
	std::string time;
	std::string dateFormat = "%F";
	std::string timeFormat = "%T";
	std::string dateTimeFormat = "%F %T";

private:
	time_t now;

private:
	std::mutex mutex;
};