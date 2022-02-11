#pragma once

#define CORE_CONSOLE_LOG GET_INSTANCE(CoreLogManager<CoreConsoleLogger>)
#define CORE_FILE_LOG GET_INSTANCE(CoreLogManager<CoreFileLogger>)

#define CORE_ALL_LOG(LOG_TYPE, LOG_MESSAGE) \
		CORE_CONSOLE_LOG.Log(LOG_TYPE, LOG_MESSAGE); \
		CORE_FILE_LOG.Log(LOG_TYPE, LOG_MESSAGE);

#if _DEBUG
#define CORE_LOG CORE_FILE_LOG
#else
#define CORE_LOG CORE_FILE_LOG
#endif // _DEBUG

template<typename T>
class CoreLogManager
{
	DECLARE_SINGLETON(CoreLogManager<T>)

public:
	std::string MakeLog(const LogType logType, std::string_view logMessage, std::string_view file, const char* function, const size_t line);

	void Log(std::string_view logMessage);
	void Log(const LogType logType, std::string_view logMessage);
	void Log(const LogType logType, const size_t oid, std::string_view logMessage);

private:
	std::unique_ptr<CoreLogger> logger;
	std::shared_mutex mutex;
};

IMPLEMENT_TEMPLATE_SINGLETON(CoreLogManager)

template<typename T>
void CoreLogManager<T>::Init(void)
{
	logger = std::make_unique<T>();
}

template<typename T>
void CoreLogManager<T>::Release(void)
{
	GetInstance().~CoreLogManager<T>();
}

template<typename T>
std::string CoreLogManager<T>::MakeLog(const LogType logType, std::string_view logMessage, std::string_view file, const char* function, const size_t line)
{
	WRITE_LOCK(mutex);
	return logger->MakeLog(logType, logMessage, file, function, line);
}

template<typename T>
void CoreLogManager<T>::Log(std::string_view logMessage)
{
	WRITE_LOCK(mutex);
	logger->Log(logMessage);
}

template<typename T>
void CoreLogManager<T>::Log(const LogType logType, std::string_view logMessage)
{
	WRITE_LOCK(mutex);
	logger->Log(logType, logMessage);
}

template<typename T>
void CoreLogManager<T>::Log(const LogType logType, const size_t oid, std::string_view logMessage)
{
	WRITE_LOCK(mutex);
	logger->Log(logType, oid, logMessage);
}