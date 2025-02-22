#include "CoreInclude.h"

CoreConsoleLogger::CoreConsoleLogger()
{

}

CoreConsoleLogger::~CoreConsoleLogger()
{

}

void CoreConsoleLogger::Init(void)
{
	CoreLogger::Init();
}

void CoreConsoleLogger::Log(std::string_view logMessage)
{
	std::cout << CORE_TIME_MANAGER.GetDateTime() << this->delimiter << logMessage << '\n';
}

void CoreConsoleLogger::Log(const LogType logType, std::string_view logMessage)
{
	MakeLog(logType, logMessage);

	std::cout << CORE_TIME_MANAGER.GetDateTime() << this->delimiter << this->log << '\n';
}