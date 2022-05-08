#include "Include.h"

#define CURL_STATICLIB
#pragma comment(lib, "wldap32.lib")

#if _DEBUG
#pragma comment(lib, "libcurld.lib")
#else
#pragma comment(lib, "libcurl.lib")
#endif // _DEBUG

#include "curl/include/curl.h"

IMPLEMENT_SINGLETON(WebManager)

void WebManager::Init(void)
{
	
}

void WebManager::Release(void)
{
	GetInstance().~WebManager();
}