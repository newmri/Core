#include "Include.h"

IMPLEMENT_SINGLETON(LoginServerManager)

thread_local std::shared_ptr<AccountDB> LoginServerManager::accountDB;

void LoginServerManager::Init(void)
{
	this->loginServer = std::make_unique<LoginServer>(7777);
	CSV_LOAD_AND_TO_HAS_MAP("DBConfig.csv", DBConfig, this->dbConfig, Name);
}

void LoginServerManager::Release(void)
{
	GetInstance().~LoginServerManager();
}

void LoginServerManager::Run(void)
{
	this->loginServer->Run();
}

void LoginServerManager::Stop(void)
{
	this->loginServer->Stop();
}

std::shared_ptr<AccountDB> LoginServerManager::GetAccountDB(void)
{
	if (IS_SAME(nullptr, this->accountDB))
		MakeAccountDB();

	return this->accountDB;
}

void LoginServerManager::MakeAccountDB(void)
{
	auto begin = this->dbConfig.cbegin();
	auto end = this->dbConfig.cend();
	for (; begin != end; ++begin)
	{
		CORE_LOG.Log(LogType::LOG_DEBUG, "[DBName]: " + begin->second->Name);
		CORE_LOG.Log(LogType::LOG_DEBUG, "[DB IP]: " + begin->second->IP);

		if (ENUM_TO_STR(Account) == begin->second->Name)
			this->accountDB = std::make_shared<AccountDB>(std::wstring(begin->second->Name.begin(), begin->second->Name.end()));
	}
}