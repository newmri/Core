#include "CoreInclude.h"

CoreLogger::CoreLogger()
{

}

CoreLogger::~CoreLogger()
{

}

void CoreLogger::Init(void)
{
	this->log.reserve(CORE_BIG_SIZE);
	this->dummyStr.reserve(CORE_BIG_SIZE);
}

void CoreLogger::MakeLog(const LogType logType)
{
	this->log = this->logHeader[static_cast<size_t>(logType)] + this->delimiter;
}

void CoreLogger::MakeLog(const LogType logType, const size_t oid)
{
	MakeLog(logType);
	this->log += "oid(" + TO_STR(oid) + ")" + this->delimiter;
}

void CoreLogger::MakeLog(const LogType logType, std::string_view logMessage)
{
	MakeLog(logType);
	this->log.append(logMessage);
}

void CoreLogger::MakeLog(const LogType logType, const size_t oid, std::string_view logMessage)
{
	MakeLog(logType, oid);
	this->log.append(logMessage);
}

std::string CoreLogger::MakeLog(const LogType logType, std::string_view logMessage, std::string_view file, const char* function, const size_t line)
{
	MakeLog(logType, logMessage);

	this->output.str(this->dummyStr);

	this->output << this->log;
	this->output << file << this->delimiter;
	this->output << function << this->delimiter;
	this->output << line << this->delimiter;

	return this->output.str();
}