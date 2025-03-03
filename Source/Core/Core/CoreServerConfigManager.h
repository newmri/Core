#pragma once

#define CORE_SERVER_CONFIG GET_INSTANCE(CoreServerConfigManager)

class CoreServerConfigManager
{
	DECLARE_SINGLETON(CoreServerConfigManager)

public:
	const std::shared_ptr<ServerConfig> GetServerConfig() const;
	const std::shared_ptr<ServerInfo> GetServerInfo() const;

private:
	std::shared_ptr<ServerConfig> serverConfig;
	std::shared_ptr<ServerInfo> serverInfo = std::make_shared<ServerInfo>();
};