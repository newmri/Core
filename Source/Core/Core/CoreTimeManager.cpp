#include "CoreInclude.h"

IMPLEMENT_SINGLETON(CoreTimeManager)

void CoreTimeManager::Init(void)
{

}

void CoreTimeManager::Release(void)
{
	GetInstance().~CoreTimeManager();
}

TIME_POINT CoreTimeManager::GetNow(void) const
{
	return std::chrono::system_clock::now();
}

TIME_VALUE CoreTimeManager::GetNowSeconds(void) const
{
	return std::chrono::duration_cast<std::chrono::seconds>(GetNow().time_since_epoch()).count();
}

TIME_VALUE CoreTimeManager::GetEndSeconds(TIME_VALUE limitTime) const
{
	TIME_VALUE endSeconds = limitTime;

	if (endSeconds)
		endSeconds += GetNowSeconds();

	return endSeconds;
}

void CoreTimeManager::Start(void)
{
	this->mutex.lock();
	this->start = GetNow();
}

void CoreTimeManager::End(void)
{
	this->end = GetNow();

	this->sec = this->end - this->start;

	this->time = timeEndMessage + TO_STR(this->sec.count());

	CORE_LOG.Log(LogType::LOG_INFO, this->time);

	this->mutex.unlock();
}

void CoreTimeManager::GetTime(std::string_view format, std::string& time)
{
	auto now = std::chrono::system_clock::to_time_t(GetNow());
	struct tm date;

	localtime_s(&date, &now);

	char timeBuffer[timeBufferSize] = {};

	strftime(timeBuffer, this->timeBufferSize, format.data(), &date);

	time = timeBuffer;
}

std::string CoreTimeManager::GetDate(void)
{
	std::string time;
	GetTime(this->dateFormat, time);
	return time;
}

std::string CoreTimeManager::GetTime(void)
{
	std::string time;
	GetTime(this->timeFormat, time);
	return time;
}

std::string CoreTimeManager::GetDateTime(void)
{
	std::string time;
	GetTime(this->dateTimeFormat, time);
	return time;
}