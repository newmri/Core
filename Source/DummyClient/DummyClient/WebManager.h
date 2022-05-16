#pragma once

#define WEB_MANAGER GET_INSTANCE(WebManager)

#define CURL_STATICLIB
#pragma comment(lib, "wldap32.lib")

#if _DEBUG
#pragma comment(lib, "libcurld.lib")
#else
#pragma comment(lib, "libcurl.lib")
#endif // _DEBUG

#include "curl/include/curl.h"

class WebManager
{
	DECLARE_SINGLETON(WebManager)

public:
	void Clear(CURL* curl, curl_slist* headers);

private:
	static size_t curl_callback(void* ptr, size_t size, size_t nmemb, std::string* data);

public:
	void GetWorldList(void);

public:
	void Signup(std::shared_ptr<CoreServerSession> session);
	bool Login(std::shared_ptr<CoreServerSession> session);
	ServerConnectInfo SelectServer(const Define::ServerType serverType);

public:
	std::string Get(const char* path);
	std::string Post(std::string_view data, const char* path);
};