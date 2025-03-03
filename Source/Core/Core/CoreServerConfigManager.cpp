#include "CoreInclude.h"

IMPLEMENT_SINGLETON(CoreServerConfigManager)

void CoreServerConfigManager::Init(void)
{
	CSV_LOAD_ONE_ROW("ServerConfig.csv", ServerConfig, this->serverConfig);

	CORE_LOG(LogType::LOG_DEBUG, "[Server Type]: {}", this->serverConfig->serverType);
	CORE_LOG(LogType::LOG_DEBUG, "[GroupID]: {}", this->serverInfo->groupID);
	CORE_LOG(LogType::LOG_DEBUG, "[CountryID]: {}", ENUM_TO_STR(this->serverConfig->countryID));
	CORE_LOG(LogType::LOG_DEBUG, "[WorldID]: {}", this->serverConfig->worldID);
	CORE_LOG(LogType::LOG_DEBUG, "[ServerID]: {}", this->serverConfig->serverID);
}

void CoreServerConfigManager::Release(void)
{
	GetInstance().~CoreServerConfigManager();
}

const std::shared_ptr<ServerConfig> CoreServerConfigManager::GetServerConfig() const
{
	return this->serverConfig;
}

const std::shared_ptr<ServerInfo> CoreServerConfigManager::GetServerInfo() const
{
	return this->serverInfo;
}
