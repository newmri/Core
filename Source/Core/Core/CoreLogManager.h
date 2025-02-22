#pragma once

#define CORE_CONSOLE_LOG GET_INSTANCE(CoreLogManager<CoreConsoleLogger>)
#define CORE_FILE_LOG GET_INSTANCE(CoreLogManager<CoreFileLogger>)

#if _DEBUG
#define CORE_LOG_INSTANCE CORE_FILE_LOG
#else
#define CORE_LOG_INSTANCE CORE_FILE_LOG
#endif // _DEBUG

#define CORE_LOG_IMPL(instance, logType, fmt, ...) \
    instance.Log(logType, fmt, ##__VA_ARGS__)

#define CORE_LOG(logType, fmt, ...) \
    CORE_LOG_IMPL(CORE_LOG_INSTANCE, logType, fmt, ##__VA_ARGS__)

#define CORE_ALL_LOG(logType, fmt, ...) \
    CORE_LOG_IMPL(CORE_CONSOLE_LOG, logType, fmt, ##__VA_ARGS__); \
    CORE_LOG_IMPL(CORE_FILE_LOG, logType, fmt, ##__VA_ARGS__)

#define CORE_ERROR_LOG_IMPL(instance, logType, fmt, ...) \
	instance.Log(LogType::LOG_ERROR, "{}{}{}:{} ", std::format(fmt, ##__VA_ARGS__), instance.GetDelimiter(), __FUNCTION__, __LINE__);

#define CORE_ERROR_LOG(fmt, ...) \
	CORE_ERROR_LOG_IMPL(CORE_LOG_INSTANCE, logType, fmt, ##__VA_ARGS__)

#define CORE_ALL_ERROR_LOG(logType, fmt, ...) \
    CORE_ERROR_LOG_IMPL(CORE_CONSOLE_LOG, logType, fmt, ##__VA_ARGS__); \
    CORE_ERROR_LOG_IMPL(CORE_FILE_LOG, logType, fmt, ##__VA_ARGS__)

template<typename T>
class CoreLogManager
{
	DECLARE_SINGLETON(CoreLogManager<T>)

public:
	void Log(const LogType logType, std::string_view logMessage);
	template<typename...Args>
	void Log(std::format_string<Args...> fmt, Args&&...args);
	template<typename...Args>
	void Log(const LogType logType, std::format_string<Args...> fmt, Args&&...args);

	std::string GetDelimiter() const { return logger->GetDelimiter(); }

private:
	template<typename...Args>
	std::string MakeLog(std::format_string<Args...> fmt, Args&&...args);

private:
	std::unique_ptr<CoreLogger> logger;
	std::shared_mutex mutex;
};

IMPLEMENT_TEMPLATE_SINGLETON(CoreLogManager)

template<typename T>
void CoreLogManager<T>::Init(void)
{
	logger = std::make_unique<T>();
	logger->Init();
}

template<typename T>
void CoreLogManager<T>::Release(void)
{
	GetInstance().~CoreLogManager();
}

template<typename T>
void CoreLogManager<T>::Log(const LogType logType, std::string_view logMessage)
{
	WRITE_LOCK(mutex);
	logger->Log(logType, logMessage);
}

template<typename T>
template<typename...Args>
void CoreLogManager<T>::Log(std::format_string<Args...> fmt, Args&&...args)
{
	auto logMessage = MakeLog(fmt, std::forward<Args>(args)...);
	WRITE_LOCK(mutex);
	logger->Log(logMessage);
}

template<typename T>
template<typename...Args>
void CoreLogManager<T>::Log(const LogType logType, std::format_string<Args...> fmt, Args&&...args)
{
	auto logMessage = MakeLog(fmt, std::forward<Args>(args)...);
	WRITE_LOCK(mutex);
	logger->Log(logType, logMessage);
}

template<typename T>
template<typename...Args>
std::string CoreLogManager<T>::MakeLog(std::format_string<Args...> fmt, Args&&...args)
{
	return std::format(fmt, std::forward<Args>(args)...);
}