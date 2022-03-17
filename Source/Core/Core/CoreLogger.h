#pragma once

enum class LogType : size_t
{
	LOG_DEBUG,
	LOG_INFO,
	LOG_CONNECT,
	LOG_DISCONNECT,
	LOG_ERROR,
	LOG_HACK,
	LOG_TYPE_NUM
};

enum class DisconnectType : size_t
{
	SPEED_HACK
};

class CoreLogger : public CoreObject
{
	OVERRIDE_OBJECT(CoreLogger)

private:
	void MakeLog(const LogType logType);
	void MakeLog(const LogType logType, const size_t oid);

protected:
	void MakeLog(const LogType logType, std::string_view logMessage);
	void MakeLog(const LogType logType, const size_t oid, std::string_view logMessage);

public:
	std::string MakeLog(const LogType logType, std::string_view logMessage, std::string_view file, const char* function, const size_t line);

public:
	virtual void Log(std::string_view logMessage) = 0;
	virtual void Log(const LogType logType, std::string_view logMessage) = 0;
	virtual void Log(const LogType logType, const size_t oid, std::string_view logMessage) = 0;

protected:
	std::string delimiter = "\t";
	std::string log;

private:
	std::string logHeader[static_cast<size_t>(LogType::LOG_TYPE_NUM)] = { "[LOG_DEBUG]", "[LOG_INFO]", "[LOG_CONNECT]", "[LOG_DISCONNECT]", "[LOG_ERROR]", "[LOG_HACK]" };

	std::ostringstream output;
	std::string dummyStr;
};